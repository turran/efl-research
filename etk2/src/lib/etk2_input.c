/*
 * etk2_input.c
 *
 *  Created on: 28-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Input
{
	Canvas *c;
	struct
	{
		unsigned int button;
		unsigned int x;
		unsigned int y;
		Eina_Bool inside;
		Renderable *r;
	} pointer;
	struct
	{

	} keyboard;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Input * input_new(Canvas *c)
{
	Input *i;

	i = calloc(1, sizeof(Input));
	i->c = c;
	return i;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void input_feed_mouse_in(Input *i)
{
	Renderable *r;

	if (i->pointer.inside)
		return;
	i->pointer.inside = EINA_TRUE;
	r = canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	/* TODO send the event */
}

EAPI void input_feed_mouse_move(Input *i, unsigned int x, unsigned int y)
{
	Renderable *r;
	unsigned int px, py;

	if (!i->pointer.inside)
		return;

	px = i->pointer.x;
	py = i->pointer.y;
	i->pointer.x = x;
	i->pointer.y = y;
	/* TODO grabbed */
	r = canvas_renderable_get_at_coord(i->c, x, y);
	if (r == i->pointer.r)
	{
		/* send move */
		if (r)
		{
			Event_Mouse em;

			event_mouse_move_init(&em, (Object *)r, NULL, i, x, y);
			event_dispatch((Event *)&em);
		}
	}
	else
	{
		/* send out event on i->r */
		if (i->pointer.r)
		{
			Event_Mouse em;

			event_mouse_out_init(&em, (Object *)i->pointer.r, (Object *)r, i);
			event_dispatch((Event *)&em);
		}
		/* send in event on r */
		if (r)
		{
			Event_Mouse em;

			event_mouse_in_init(&em, (Object *)r, (Object *)i->pointer.r, i);
			event_dispatch((Event *)&em);

		}
	}
	/* update the current inside */
	i->pointer.r = r;
}

EAPI void input_feed_mouse_out(Input *i)
{
	Renderable *r;

	if (!i->pointer.inside)
		return;
	i->pointer.inside = EINA_FALSE;
	r = canvas_renderable_get_at_coord(i->c, i->pointer.x, i->pointer.y);
	if (!r)
		return;
	/* TODO send the event */
}
