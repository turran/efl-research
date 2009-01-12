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
	Eina_List *renderables;
	Eina_Inlist *inputs;
};

Eina_Bool _appendable(const char *type)
{
	printf("appendable %s\n", type);
	return EINA_TRUE;

}

void _child_append_cb(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;

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
	printf("called\n");
}

static void canvas_ctor(void *instance)
{
	Canvas *canvas;
	Canvas_Private *prv;

	canvas = (Canvas*) instance;
	canvas->private = prv = type_instance_private_get(canvas_type_get(), instance);
	/* FIXME just for testing */
	((Object *)canvas)->appendable = _appendable;
	/* register to an event where some child is appended to this parent */
	event_listener_add((Object *)canvas, EVENT_OBJECT_APPEND, _child_append_cb, EINA_TRUE);
	printf("[canvas] ctor %p %p\n", canvas, canvas->private);
}

static void canvas_dtor(void *canvas)
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
				sizeof(Canvas_Private), object_type_get(), canvas_ctor, canvas_dtor);
	}

	return canvas_type;
}

Canvas * canvas_new(void)
{
	Canvas *canvas;

	canvas = type_instance_new(canvas_type_get());

	return canvas;
}

/**
 * @brief Marks a rectangle on the canvas as damaged, this area will be
 * processed again. When the canvas process that area it will no longer be
 * a damaged area
 * @param r Rectangle that defines the area damaged
 */
EAPI void canvas_damage_add(Canvas *c, Eina_Rectangle *r)
{
	//_damage_add(c, r);
}
/**
 * @brief Marks a rectangle area on the canvas that will never be processed.
 * The area is kept on the canvas until it is cleared with
 * canvas_obscure_del()
 * @param r Rectangle that defines the obscure area
 */
EAPI void canvas_obscure_add(Canvas *c, Eina_Rectangle *r)
{

}
