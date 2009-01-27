/*
 * dummy_canvas.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Canvas_Private *)((Dummy_Canvas *)(d))->private)

#define DUMMY_CANVAS_TYPE_NAME "DummyCanvas"

struct _Dummy_Canvas_Private
{
	/* track parent canvas, if parent canvas == NULL then this is the
	 * main canvas and we should treat it differently
	 */
	SDL_Surface *s;
	Eina_Bool root;
};

static Eina_Bool _flush(Canvas *c, Eina_Rectangle *r)
{
	Dummy_Canvas_Private *prv;

	prv = PRIVATE(c);
	/* in case of root flip
	 * otherwise blt
	 */
	SDL_Flip(prv->s);
}

static void _prop_modify_cb(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	/* check if the change is the rectangle */
	if ((em->state == EVENT_MUTATION_STATE_POST) && (!strcmp(em->prop, "geometry")))
	{
		Dummy_Canvas_Private *prv = PRIVATE(o);

		printf("CANVAS SIZE SET %d %d\n", em->curr->value.rect.w, em->curr->value.rect.h);
		prv->s = SDL_SetVideoMode(em->curr->value.rect.w,
			em->curr->value.rect.h, 32,
			SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF);
	}

}

static void _ctor(void *instance)
{
	Dummy_Canvas *dc;
	Dummy_Canvas_Private *prv;
	static int created = 0;

	dc = (Dummy_Canvas*) instance;
	dc->private = prv = type_instance_private_get(dummy_canvas_type_get(), instance);
	dc->parent.flush = _flush;
	/* FIXME easy hack for now, we should do this better */
	if (!created)
	{
		created = 1;
	}
	/* TODO Set up the callbacks */
	object_event_listener_add((Object *)dc, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE);
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type *dummy_canvas_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_CANVAS_TYPE_NAME, sizeof(Dummy_Canvas),
				sizeof(Dummy_Canvas_Private), canvas_type_get(), _ctor, _dtor);
	}

	return type;
}

Dummy_Canvas * dummy_canvas_new(void)
{
	return type_instance_new(dummy_canvas_type_get());
}

void dummy_canvas_resize(Dummy_Canvas *c, int w, int h)
{
	//printf("[DummyCanvas] %p\n", renderable_canvas_get((Renderable *)c));
	canvas_size_set((Canvas *)c, w, h);
	//printf("[DummyCanvas] %p\n", renderable_canvas_get((Renderable *)c));
	/* set the property */
	/* on set cb, check if w && h != 0, if so
	 * create the surface
	 */
}

SDL_Surface * dummy_canvas_surface_get(Dummy_Canvas *c)
{
	Dummy_Canvas_Private *prv;

	prv = PRIVATE(c);
	return prv->s;
}
