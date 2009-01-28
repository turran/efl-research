/*
 * etk2_event.c
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void event_mutation_init(Event_Mutation *em, const char *type, const Object *o,
		const Object *rel, const Property *prop, Value *prev, Value *curr,
		Event_Mutation_State state)
{
	event_init((Event *)em, type, o, EINA_TRUE);
	em->related = rel;
	em->prev = prev;
	em->curr = curr;
	em->state = state;
	if (prop)
	{
		em->prop = property_name_get(prop);
		em->prop_id = property_id_get(prop);
	}
}
void event_ui_init(Event_Ui *eui, const char *type, const Object *o,
		const Object *related, const Input *i)
{
	event_init((Event *)eui, type, o, EINA_FALSE);
	eui->related = related;
	eui->i = i;
}

void event_mouse_move_init(Event_Mouse *em, const Object *o,
		const Object *related, const Input *i,
		unsigned int sx, unsigned int sy)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_MOVE, o, related, i);
	em->screen.x = sx;
	em->screen.y = sy;
}

void event_mouse_in_init(Event_Mouse *em, const Object *o, const Object *related,
		const Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_IN, o, related, i);
}
void event_mouse_out_init(Event_Mouse *em, const Object *o, const Object *related,
		const Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_OUT, o, related, i);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Adds a listener for the event type ...
 */
EAPI void event_listener_add(Object *o, const char *type,
		Event_Listener el, Eina_Bool bubble)
{
	object_event_listener_add(o, type, el, bubble);
}

EAPI void event_listener_remove(Object *o, const char *type,
		Event_Listener el)
{
	object_event_listener_remove(o, type, el);
}

EAPI void event_dispatch(Event *e)
{
	object_event_dispatch(e->target, e);
}

EAPI void event_init(Event *e, const char *type, const Object *o, Eina_Bool bubbles)
{
	e->target = o;
	e->type = type;
	e->bubbles = bubbles;
}
#if 0
EAPI void event_property_int_changed(Event *e, const char *type,
		const Object *o, int prev, int curr)
{

}
#endif

