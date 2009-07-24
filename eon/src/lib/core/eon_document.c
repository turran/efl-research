/*
 * eon_document.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
/*
 * TODO remove the engine from the list of attributes
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Document_Private *)((Eon_Document *)(d))->private)

static Ecore_Idle_Enterer *_idler = NULL;
static Eina_List *_documents = NULL;

struct _Eon_Document_Private
{
	struct {
		char *name;
		Eon_Engine *backend;
	} engine;
	Eina_Rectangle size;
	Eon_Canvas *canvas;
	Eina_Hash *ids;
	Etch *etch;
	Ecore_Timer *anim_cb;
};

/* Called whenever an object changes it's id */
static void _id_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Document_Private *prv = PRIVATE(data);

	eina_hash_add(prv->ids, em->curr->value.string_value, o);
}

static int _idler_cb(void *data)
{
	Eina_Iterator *it;
	Eon_Document *doc;

	it = eina_list_iterator_new(_documents);
	while (eina_iterator_next(it, (void **)&doc))
	{
		ekeko_object_process((Ekeko_Object *)doc);
	}
	return 1;
}
static int _animation_cb(void *data)
{
	Eon_Document *doc = data;
	Eon_Document_Private *prv = PRIVATE(doc);

	etch_timer_tick(prv->etch);
	return 1;
}
/* Called whenever a child is appended to the document */
static void _child_append_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Document *d = (Eon_Document *)o;
	Eon_Document_Private *prv = PRIVATE(d);

	ekeko_event_listener_add(e->target, EKEKO_OBJECT_ID_CHANGED, _id_change, EINA_FALSE, d);
	/* check that the child is of type target */
	if (!ekeko_type_instance_is_of(e->target, EON_TYPE_CANVAS))
		return;
	/* if we dont have a canvas yet call the document engine */
	if (!prv->canvas)
	{
		prv->canvas = (Eon_Canvas *)e->target;
		eon_engine_document_create(prv->engine.backend, d);
	}
	/* TODO set the engine automatically */
	/* TODO what happens if the user doesnt set the engine and then appends
	 * a canvas
	 */
}

static void _prop_modify_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Document *d = (Eon_Document *)o;
	Eon_Document_Private *prv = PRIVATE(d);

	/* When the engine property changes set it up */
	if (!strcmp(em->prop, "engine") && !prv->engine.backend)
	{
		prv->engine.backend = eon_engine_get(em->curr->value.string_value);
		eon_engine_ref(prv->engine.backend, d);
	}
}

static void _ctor(void *instance)
{
	Eon_Document *dc;
	Eon_Document_Private *prv;

	/* FIXME do we need a single idler or better one idler per document? */
	if (!_idler)
	{
		/* this idler will process every child */
		_idler = ecore_idle_enterer_add(_idler_cb, NULL);
	}
	dc = (Eon_Document*) instance;
	dc->private = prv = ekeko_type_instance_private_get(eon_document_type_get(), instance);
	/* setup the animation system */
	prv->etch = etch_new();
	etch_timer_fps_set(prv->etch, 30);
	prv->anim_cb = ecore_timer_add(1.0f/30.0f, _animation_cb, dc);
	/* the id system */
	prv->ids = 	eina_hash_string_superfast_new(NULL);
	/* the event listeners */
	ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_OBJECT_APPEND, _child_append_cb, EINA_TRUE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)dc, EKEKO_EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE, NULL);
}

static void _dtor(void *canvas)
{

}

static Eina_Bool _appendable(void *parent, void *child)
{
	Eon_Document_Private *prv = PRIVATE(parent);

	if (prv->canvas)
		return EINA_FALSE;
	if (!ekeko_type_instance_is_of(child, EON_TYPE_CANVAS))
		return EINA_FALSE;
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Engine * eon_document_engine_get(Eon_Document *d)
{
	Eon_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->engine.backend;
}
Etch * eon_document_etch_get(Eon_Document *d)
{
	Eon_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->etch;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_DOCUMENT_SIZE;

Ekeko_Type *eon_document_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_DOCUMENT, sizeof(Eon_Document),
				sizeof(Eon_Document_Private), ekeko_object_type_get(), _ctor,
				_dtor, _appendable);
		TYPE_PROP_SINGLE_ADD(type, "engine", PROPERTY_STRING, OFFSET(Eon_Document_Private, engine.name));
		EON_DOCUMENT_SIZE = TYPE_PROP_SINGLE_ADD(type, "size", PROPERTY_RECTANGLE, OFFSET(Eon_Document_Private, size));
	}

	return type;
}

EAPI Eon_Document * eon_document_new(const char *engine, int w, int h, const char *options)
{
	Eon_Document *d;
	Eon_Canvas *c;
	Ekeko_Value v;

	d = ekeko_type_instance_new(eon_document_type_get());
	ekeko_value_str_from(&v, engine);
	ekeko_object_property_value_set((Ekeko_Object *)d, "engine", &v);
	/* the main canvas */
	c = ekeko_type_instance_new(eon_canvas_type_get());
	ekeko_object_child_append((Ekeko_Object *)d, (Ekeko_Object *)c);
	_documents = eina_list_append(_documents, d);
	eon_document_resize(d, w, h);

	return d;
}

EAPI Eon_Canvas * eon_document_canvas_get(Eon_Document *d)
{
	Eon_Document_Private *prv;

	prv = PRIVATE(d);
	return prv->canvas;
}

EAPI void eon_document_size_get(Eon_Document *d, int *w, int *h)
{
	Eon_Document_Private *prv;

	prv = PRIVATE(d);
	if (w) *w = prv->size.w;
	if (h) *h = prv->size.h;
}

EAPI void eon_document_resize(Eon_Document *d, int w, int h)
{
	Ekeko_Value v;

	ekeko_value_rectangle_coords_from(&v, 0, 0, w, h);
	ekeko_object_property_value_set((Ekeko_Object *)d, "size", &v);
}

EAPI Ekeko_Object * eon_document_object_get_by_id(Eon_Document *d, const char *id)
{
	Eon_Document_Private *prv = PRIVATE(d);

	return eina_hash_find(prv->ids, id);
}

EAPI void eon_document_pause(Eon_Document *d)
{
	Eon_Document_Private *prv = PRIVATE(d);
	if (!prv->anim_cb)
		return;

	ecore_timer_del(prv->anim_cb);
	prv->anim_cb = NULL;
}

EAPI void eon_document_play(Eon_Document *d)
{
	Eon_Document_Private *prv = PRIVATE(d);
	if (prv->anim_cb)
		return;

	prv->anim_cb = ecore_timer_add(1.0f/30.0f, _animation_cb, d);
}