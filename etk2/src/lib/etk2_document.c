/*
 * etk2_document.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Document_Private *)((Etk_Document *)(d))->private)

static Ecore_Idle_Enterer *_idler = NULL;
static Eina_List *_documents = NULL;

struct _Etk_Document_Private
{
	struct {
		char *name;
		Etk_Engine *func;
	} engine;
	Eina_Rectangle size;
	Etk_Canvas *canvas;
	Etch *etch;
	Eina_Hash *ids;
};

/* Called whenever an object changes it's id */
static void _id_change(const Ekeko_Object *o, Event *e, void *data)
{
	printf("Setting id\n");
}

static int _idler_cb(void *data)
{
	Eina_Iterator *it;
	Etk_Document *doc;

	it = eina_list_iterator_new(_documents);
	while (eina_iterator_next(it, (void **)&doc))
	{
		ekeko_object_process((Ekeko_Object *)doc);
	}
	return 1;
}
static int _animation_cb(void *data)
{
	Etk_Document *doc = data;
	Etk_Document_Private *prv = PRIVATE(doc);

	etch_timer_tick(prv->etch);
	return 1;
}
/* Called whenever a child is appended to the document */
static void _child_append_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Document *d = (Etk_Document *)o;
	Etk_Document_Private *prv = PRIVATE(d);

	/* check that the child is of type target */
	if (!ekeko_type_instance_is_of(e->target, ETK_TYPE_CANVAS))
		return;
	/* if we dont have a canvas yet call the document engine */
	if (!prv->canvas)
	{
		prv->canvas = (Etk_Canvas *)e->target;
		prv->engine.func->doc->create(d);
	}
	/* TODO set the engine automatically */
	/* TODO what happens if the user doesnt set the engine and then appends
	 * a canvas
	 */
}

static void _prop_modify_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Document *d = (Etk_Document *)o;
	Etk_Document_Private *prv = PRIVATE(d);

	/* When the engine property changes set it up */
	if (!strcmp(em->prop, "engine"))
	{
		/* TODO how to handle this case: we dont want the engine to
		 * be set more than once!
		 */
		prv->engine.func = etk_engine_get(em->curr->value.string_value);
	}
}

static void _ctor(void *instance)
{
	Etk_Document *dc;
	Etk_Document_Private *prv;

	/* FIXME do we need a single idler or better one idler per document? */
	if (!_idler)
	{
		/* this idler will process every child */
		_idler = ecore_idle_enterer_add(_idler_cb, NULL);
	}
	dc = (Etk_Document*) instance;
	dc->private = prv = ekeko_type_instance_private_get(etk_document_type_get(), instance);
	/* setup the animation system */
	prv->etch = etch_new();
	ecore_timer_add(1.0f/30.0f, _animation_cb, dc);
	etch_timer_fps_set(prv->etch, 30);
	ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_OBJECT_APPEND, _child_append_cb, EINA_TRUE, NULL);
	//ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_OBJECT_ID_CHANGED, _id_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE, NULL);

}

static void _dtor(void *canvas)
{

}

static Eina_Bool _appendable(void *parent, void *child)
{
	Etk_Document_Private *prv = PRIVATE(parent);

	if (prv->canvas)
		return EINA_FALSE;
	if (!ekeko_type_instance_is_of(child, ETK_TYPE_CANVAS))
		return EINA_FALSE;
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Engine * etk_document_engine_get(Etk_Document *d)
{
	Etk_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->engine.func;
}
Etch * etk_document_etch_get(Etk_Document *d)
{
	Etk_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->etch;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Property_Id ETK_DOCUMENT_SIZE;

Ekeko_Type *etk_document_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_DOCUMENT, sizeof(Etk_Document),
				sizeof(Etk_Document_Private), ekeko_object_type_get(), _ctor,
				_dtor, _appendable);
		TYPE_PROP_SINGLE_ADD(type, "engine", PROPERTY_STRING, OFFSET(Etk_Document_Private, engine.func));
		ETK_DOCUMENT_SIZE = TYPE_PROP_SINGLE_ADD(type, "size", PROPERTY_RECTANGLE, OFFSET(Etk_Document_Private, size));
	}

	return type;
}
EAPI Etk_Document * etk_document_new(const char *engine, int w, int h)
{
	Etk_Document *d;
	Etk_Canvas *c;
	Ekeko_Value v;

	d = ekeko_type_instance_new(etk_document_type_get());
	ekeko_value_str_from(&v, engine);
	ekeko_object_property_value_set((Ekeko_Object *)d, "engine", &v);
	c = ekeko_type_instance_new(etk_canvas_type_get());
	ekeko_object_child_append((Ekeko_Object *)d, (Ekeko_Object *)c);
	_documents = eina_list_append(_documents, d);
	etk_document_resize(d, w, h);

	return d;
}

EAPI Etk_Canvas * etk_document_canvas_get(Etk_Document *d)
{
	Etk_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->canvas;
}

EAPI void etk_document_size_get(Etk_Document *d, int *w, int *h)
{
	Etk_Document_Private *prv;

	prv = PRIVATE(d);
	if (w) *w = prv->size.w;
	if (h) *h = prv->size.h;
}

EAPI void etk_document_resize(Etk_Document *d, int w, int h)
{
	Ekeko_Value v;

	ekeko_value_rectangle_coords_from(&v, 0, 0, w, h);
	ekeko_object_property_value_set((Ekeko_Object *)d, "size", &v);
}

static _id_get(Ekeko_Object *o, const char *id)
{

}

static Ekeko_Object * _iterate(Ekeko_Object *o, const char *id, int level)
{
	Ekeko_Object *child;

	child = ekeko_object_child_first_get(o);
	if (!child)
		return NULL;
	do
	{
		char *cid;
		Ekeko_Object *found;
		int i;

		cid = ekeko_object_id_get(child);
		if (cid)
		{
			printf("%s\n", cid);
		}
		if (cid && (!strcmp(cid, id)))
			return child;

		found = _iterate(child, id, level + 1);
		if (found) return found;

		child = ekeko_object_next(child);
		for (i = 0; i < level; i++)
		{
			printf("\t");
		}
		printf("child %p\n", child);
	} while (child);
	return NULL;
#if 0
	child = ekeko_object_child_first_get(o);
	curr = child;

	if (!child) return NULL;
	while (child)
	{
		char *cid;

		cid = ekeko_object_id_get(child);
		if (cid)
		{
			printf("%s\n", cid);
		}
		if (cid && (!strcmp(cid, id)))
			return child;
		child = ekeko_object_next(child);
		_iterate(curr, id);;
	}
	return child;
#endif
}


EAPI Ekeko_Object * etk_document_object_get_by_id(Etk_Document *d, const char *id)
{
	Ekeko_Object *c;

	/* FIXME the document should store every id on its hash */
	/* for now we iterate through the childs */
	printf("Looking for %s\n", id);
	c = _iterate((Ekeko_Object *)d, id, 0);
	return c;
}

