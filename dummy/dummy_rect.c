/*
 * dummy_rect.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Rect_Private *)((Dummy_Rect *)(d))->private)

#define DUMMY_RECT_TYPE_NAME "DummyRect"


struct _Dummy_Rect_Private
{
	int color;
};

static void _render(Renderable *r, Eina_Rectangle *rect)
{
	SDL_Rect srect;
	SDL_Surface *s;
	Dummy_Canvas *dc;
	Dummy_Rect_Private *prv;


	dc = (Dummy_Canvas *)renderable_canvas_get(r);
	srect.x = rect->x;
	srect.y = rect->y;
	srect.w = rect->w;
	srect.h = rect->h;

	s = dummy_canvas_surface_get(dc);
	prv = PRIVATE(r);;
	SDL_FillRect(s, &srect, prv->color);
	printf("[DummyRect] rendering into %p %d %d %d %d with color 0x%08x\n", dc, srect.x, srect.y, srect.w, srect.h, prv->color);
}


static void _ctor(void *instance)
{
	Dummy_Rect *dc;
	Dummy_Rect_Private *prv;

	dc = (Dummy_Rect*) instance;
	dc->private = prv = type_instance_private_get(dummy_rect_type_get(), instance);
	dc->parent.render = _render;
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type * dummy_rect_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_RECT_TYPE_NAME, sizeof(Dummy_Rect),
				sizeof(Dummy_Rect_Private), renderable_type_get(), _ctor, _dtor);
	}

	return type;
}
Dummy_Rect * dummy_rect_new(void)
{
	return type_instance_new(dummy_rect_type_get());
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Dummy_Rect * dummy_rect_add(Dummy_Canvas *dc)
{
	Dummy_Rect *dr;

	dr = dummy_rect_new();
	/* attach the rect to the canvas */
	object_child_append((Object *)dc, (Object *)dr);
	return dr;
}

void dummy_rect_move(Dummy_Rect *r, int x, int y)
{
	renderable_move((Renderable *)r, x, y);
}

void dummy_rect_resize(Dummy_Rect *r, int w, int h)
{
	renderable_resize((Renderable *)r, w, h);
}

void dummy_rect_color_set(Dummy_Rect *r, int color)
{
	Dummy_Rect_Private *prv;

	prv = PRIVATE(r);
	prv->color = color;
	printf("[DummyRect] color set 0x%08x\n", prv->color);
}
