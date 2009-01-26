/*
 * etk2_canvas.c
 *
 *  Created on: 08-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(canvas) ((Canvas_Private*)(canvas->private))

#define TYPE_NAME "Canvas"

struct _Canvas_Private
{
	Eina_Tiler *tiler;
	Eina_List *renderables;
	/* TODO obscures */
	Eina_Inlist *inputs;
};

Eina_Bool _appendable(const char *type)
{
	printf("appendable %s\n", type);
	return EINA_TRUE;

}

static void _prop_modify_cb(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	/* check if the change is the rectangle */
	if ((em->state == EVENT_MUTATION_STATE_POST) && (!strcmp(em->prop, "geometry")))
	{
		Canvas_Private *prv;
		Eina_Tiler *tiler;

		prv = PRIVATE(((Canvas *)o));
		tiler = prv->tiler;
		if (tiler)
		{
			eina_tiler_del(tiler);
		}
		printf("Changing geometry!!!!!!!!!!\n");
		prv->tiler = eina_tiler_new(em->curr->value.rect.w, em->curr->value.rect.h);
		/* TODO In case it already has a tiler, mark everything again */
		if (tiler)
		{

		}
	}
}

/* Called whenever the process has finished on this element */
static void _processed_cb(const Object *o, Event *e)
{
	/* iterate over the list of renderables */
}

static void _child_append_cb(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Canvas_Private *prv;

	/* TODO check if object is the same as the event.rel or
	 * check if the event.target is not a canvas and it is different
	 * than this
	 */
	if (!type_instance_is_of(em->related, "Renderable"))
	{
		printf("Is not of type renderable\n");
		return;
	}
	/* TODO check that the child is actually an instance of a renderable type
	 * if so append it to the renderables
	 */
	/*
	 * if the appended child is a canvas, register every UI event to this
	 * object, so when they arrive insert those events into the new canvas
	 */
	/*
	 * What happens if the child is of type renderable *and* has renderable
	 * objects?
	 */
	prv = PRIVATE(((Canvas *)em->related));
	/* TODO add a callback when the renderable changes the geometry */
	/* TODO move the list_append to the above callback *if* the object is
	 * inside the canvas geometry
	 */
	prv->renderables = eina_list_append(prv->renderables, o);

	printf("called\n");
}

static void _ctor(void *instance)
{
	Canvas *canvas;
	Canvas_Private *prv;

	canvas = (Canvas*) instance;
	canvas->private = prv = type_instance_private_get(canvas_type_get(), instance);
	/* FIXME just for testing */
	((Object *)canvas)->appendable = _appendable;
	prv->renderables = NULL;
	/* register to an event where some child is appended to this parent */
	event_listener_add((Object *)canvas, EVENT_OBJECT_APPEND, _child_append_cb, EINA_TRUE);
	/* register the event where the size is changed */
	event_listener_add((Object *)canvas, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE);
	/* TODO add the event listener when the object has finished the process() function */
	printf("[canvas] ctor %p %p\n", canvas, canvas->private);
}

static void _dtor(void *canvas)
{
	printf("[canvas] dtor %p\n", canvas);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *canvas_type_get(void)
{
	static Type *canvas_type = NULL;

	if (!canvas_type)
	{
		canvas_type = type_new(TYPE_NAME, sizeof(Canvas),
				sizeof(Canvas_Private), renderable_type_get(), _ctor, _dtor);
	}

	return canvas_type;
}

Canvas * canvas_new(void)
{
	Canvas *canvas;

	canvas = type_instance_new(canvas_type_get());

	return canvas;
}

void canvas_size_set(Canvas *c, int w, int h)
{
	Eina_Rectangle rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;
	renderable_geometry_set((Renderable *)c, &rect);
}
/**
 * @brief Marks a rectangle on the canvas as damaged, this area will be
 * processed again. When the canvas process that area it will no longer be
 * a damaged area
 * @param r Rectangle that defines the area damaged
 */
EAPI void canvas_damage_add(Canvas *c, Eina_Rectangle *r)
{
	Canvas_Private *prv;

	prv = PRIVATE(c);
	if (prv->tiler)
	{
		//_damage_add(c, r);
	}
}
/**
 * @brief Marks a rectangle area on the canvas that will never be processed.
 * The area is kept on the canvas until it is cleared with
 * canvas_obscure_del()
 * @param r Rectangle that defines the obscure area
 */
EAPI void canvas_obscure_add(Canvas *c, Eina_Rectangle *r)
{
	Canvas_Private *prv;

	prv = PRIVATE(c);
	//_obscures_add(c, r);
}

/* TODO
 * do every feed_mouse_xxx
 */
