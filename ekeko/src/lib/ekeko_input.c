/*
 * etk2_input.c
 *
 *  Created on: 28-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Input
{
	Ekeko_Canvas *c;
	struct
	{
		unsigned int downx;
		unsigned int downy;
		unsigned int button;
		unsigned int x;
		unsigned int y;
		Eina_Bool inside;
		Ekeko_Renderable *r;
	} pointer;
	struct
	{

	} keyboard;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Input * input_new(Ekeko_Canvas *c)
{
	Ekeko_Input *i;

	i = calloc(1, sizeof(Ekeko_Input));
	i->c = c;
	return i;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i)
{
	Ekeko_Renderable *r;

	if (i->pointer.inside)
		return;
	i->pointer.inside = EINA_TRUE;
	r = ekeko_canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	/* TODO send the event */
}

EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, unsigned int x, unsigned int y)
{
	Ekeko_Renderable *r;
	unsigned int px, py;

	if (!i->pointer.inside)
		return;

	px = i->pointer.x;
	py = i->pointer.y;
	i->pointer.x = x;
	i->pointer.y = y;
	/* TODO grabbed */
	/* TODO handle the subcanvas
	 * if mouse in an object and canvas(obj) != canvas => in canvas(obj) ?
	 * if mouse out an object and canvas(obj) != canvas => out canvas(obj) ?
	 */
	r = ekeko_canvas_renderable_get_at_coord(i->c, x, y);
	if (r == i->pointer.r)
	{
		/* send move */
		if (r)
		{
			Event_Mouse em;

			event_mouse_move_init(&em, (Ekeko_Object *)r, NULL, i, x, y);
			ekeko_event_dispatch((Event *)&em);
		}
	}
	else
	{
		/* send out event on i->r */
		if (i->pointer.r)
		{
			Event_Mouse em;

			event_mouse_out_init(&em, (Ekeko_Object *)i->pointer.r, (Ekeko_Object *)r, i);
			ekeko_event_dispatch((Event *)&em);
		}
		/* send in event on r */
		if (r)
		{
			Event_Mouse em;

			event_mouse_in_init(&em, (Ekeko_Object *)r, (Ekeko_Object *)i->pointer.r, i);
			ekeko_event_dispatch((Event *)&em);

		}
	}
	/* update the current inside */
	i->pointer.r = r;
}

EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i)
{
	Ekeko_Renderable *r;

	if (!i->pointer.inside)
		return;
	i->pointer.inside = EINA_FALSE;
	r = ekeko_canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	/* TODO send the event */
}

EAPI void ekeko_input_feed_mouse_down(Ekeko_Input *i)
{
	Ekeko_Renderable *r;
	Event_Mouse em;

	if (!i->pointer.inside)
		return;
	r = ekeko_canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	i->pointer.downx = i->pointer.x;
	i->pointer.downy = i->pointer.y;
	event_mouse_down_init(&em, (Ekeko_Object *)r, (Ekeko_Object *)i->pointer.r, i);
	ekeko_event_dispatch((Event *)&em);
}

EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i)
{
	Ekeko_Renderable *r;
	Event_Mouse em;

	if (!i->pointer.inside)
		return;
	r = ekeko_canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	event_mouse_up_init(&em, (Ekeko_Object *)r, (Ekeko_Object *)i->pointer.r, i);
	ekeko_event_dispatch((Event *)&em);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	if ((i->pointer.downx == i->pointer.x) && (i->pointer.downy == i->pointer.y))
	{
		event_mouse_click_init(&em, (Ekeko_Object *)r, (Ekeko_Object *)i->pointer.r, i);
		ekeko_event_dispatch((Event *)&em);
	}
}
