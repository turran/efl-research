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

/* in case the subcanvas has another canva as parent it will blt to the
 * parent canvas
 */
static void _subcanvas_render(Renderable *r, Eina_Rectangle *rect)
{
	Eina_Rectangle sgeom, rscaled;
	SDL_Rect srect, drect;
	Dummy_Canvas *dc;
	Dummy_Canvas_Private *sprv, *cprv;

	sprv = PRIVATE(r);
	dc = renderable_canvas_get(r);
	cprv = PRIVATE(dc);

	/* blt there */
	renderable_geometry_get(r, &sgeom);
	eina_rectangle_rescale_in(&sgeom, rect, &rscaled);

	srect.x = rscaled.x;
	srect.y = rscaled.y;
	srect.w = rscaled.w;
	srect.h = rscaled.h;

	drect.x = rect->x;
	drect.y = rect->y;
	drect.w = rect->w;
	drect.h = rect->h;

	printf("[DummyCanvas] rendering into %p from %d %d %d %d to %d %d %d %d\n",
			dc, srect.x, srect.y, srect.w, srect.h,
			drect.x, drect.y, drect.w, drect.h);
	SDL_BlitSurface(sprv->s, &srect, cprv->s, &drect);
}

static Eina_Bool _flush(Canvas *c, Eina_Rectangle *r)
{
	Dummy_Canvas_Private *prv;

	prv = PRIVATE(c);
	if (!prv->s)
	{
		printf("[DummyCanvas] the canvas doesnt have a surface\n");
		return EINA_TRUE;
	}
	/* if root flip */
	if (prv->root)
	{
		printf("[DummyCanvas] flipping root surface\n");
		SDL_Flip(prv->s);
		return EINA_TRUE;
	}
	/* otherwise blt */
	else
	{
		Eina_Rectangle rscaled;
		Eina_Rectangle cgeom;
		Canvas *dc; /* the canvas this subcanvas has */

		/* this canvas doesnt have a parent canvas? */
		dc = renderable_canvas_get((Renderable *)c);
		if (!dc)
			return EINA_TRUE;
		renderable_geometry_get((Renderable *)c, &cgeom);
		/* transform the rectangle relative to the upper canvas */
		eina_rectangle_rescale_out(&cgeom, r, &rscaled);
		printf("[DummyCanvas] subcanvas adding a new damage %d %d %d %d (%d %d %d %d)\n",
				rscaled.x, rscaled.y, rscaled.w, rscaled.h,
				r->x, r->y, r->w, r->h);
		printf("[DummyCanvas] subcanvas = %p, canvas = %p\n", c, dc);
		canvas_damage_add(dc, &rscaled);
	}
	return EINA_FALSE;
}

static void _prop_modify_cb(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	/* check if the change is the rectangle */
	if ((em->state == EVENT_MUTATION_STATE_POST) && (!strcmp(em->prop, "geometry")))
	{
		Dummy_Canvas_Private *prv = PRIVATE(o);

		if (prv->root)
		{
			printf("Setting video mode to %d %d\n", em->curr->value.rect.w, em->curr->value.rect.h);
			prv->s = SDL_SetVideoMode(em->curr->value.rect.w,
					em->curr->value.rect.h, 32,
					SDL_RESIZABLE | SDL_SRCALPHA |
					SDL_DOUBLEBUF);
		}
		else
		{
			/* create a normal surface */
			prv->s = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
					em->curr->value.rect.w,
					em->curr->value.rect.h, 32,
					0xff << 24, 0xff << 16, 0xff << 8, 0xff);
		}
	}

}

static void _ctor(void *instance)
{
	Dummy_Canvas *dc;
	Dummy_Canvas_Private *prv;

	dc = (Dummy_Canvas*) instance;
	dc->private = prv = type_instance_private_get(dummy_canvas_type_get(), instance);
	dc->parent.flush = _flush;
	dc->parent.parent.render = _subcanvas_render;
	/* TODO Set up the callbacks */
	event_listener_add((Object *)dc, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE);
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

void dummy_canvas_root_set(Dummy_Canvas *c)
{
	Dummy_Canvas_Private *prv;

	prv = PRIVATE(c);
	prv->root = EINA_TRUE;
}

void dummy_canvas_resize(Dummy_Canvas *c, int w, int h)
{
	//printf("[DummyCanvas] %p\n", renderable_canvas_get((Renderable *)c));
	canvas_size_set((Canvas *)c, w, h);
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
