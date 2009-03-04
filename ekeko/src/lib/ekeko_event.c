/*
 * etk2_event.c
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void event_mutation_init(Event_Mutation *em, const char *type, const Ekeko_Object *o,
		const Ekeko_Object *rel, const Property *prop, Ekeko_Value *prev, Ekeko_Value *curr,
		Event_Mutation_State state)
{
	ekeko_event_init((Event *)em, type, o, EINA_TRUE);
	em->related = rel;
	em->prev = prev;
	em->curr = curr;
	em->state = state;
	if (prop)
	{
		em->prop = ekeko_property_name_get(prop);
		em->prop_id = ekeko_property_id_get(prop);
	}
}
void event_ui_init(Event_Ui *eui, const char *type, const Ekeko_Object *o,
		const Ekeko_Object *related, const Ekeko_Input *i)
{
	ekeko_event_init((Event *)eui, type, o, EINA_FALSE);
	eui->related = related;
	eui->i = i;
}

void event_mouse_move_init(Event_Mouse *em, const Ekeko_Object *o,
		const Ekeko_Object *related, const Ekeko_Input *i,
		unsigned int sx, unsigned int sy)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_MOVE, o, related, i);
	em->screen.x = sx;
	em->screen.y = sy;
}

void event_mouse_in_init(Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_IN, o, related, i);
}
void event_mouse_out_init(Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_OUT, o, related, i);
}

void event_mouse_down_init(Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_DOWN, o, related, i);
}

void event_mouse_up_init(Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_UP, o, related, i);
}

void event_mouse_click_init(Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i)
{
	event_ui_init((Event_Ui*)em, EVENT_UI_MOUSE_CLICK, o, related, i);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Adds a listener for the event type ...
 */
EAPI void ekeko_event_listener_add(Ekeko_Object *o, const char *type,
		Event_Listener el, Eina_Bool bubble, void *data)
{
	object_event_listener_add(o, type, el, bubble, data);
}

EAPI void ekeko_event_listener_remove(Ekeko_Object *o, const char *type,
		Event_Listener el, Eina_Bool bubble, void *data)
{
	object_event_listener_remove(o, type, el, bubble, data);
}

EAPI void ekeko_event_dispatch(Event *e)
{
	ekeko_object_event_dispatch(e->target, e);
}

EAPI void ekeko_event_init(Event *e, const char *type, const Ekeko_Object *o, Eina_Bool bubbles)
{
	e->target = o;
	e->type = type;
	e->bubbles = bubbles;
}
