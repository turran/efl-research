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
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Adds a listener for the event type ...
 */
EAPI void event_listener_add(Object *o, const char *type,
		Event_Listener el)
{
	object_event_listener_add(o, type, el);
}

EAPI void event_listener_remove(Object *o, const char *type,
		Event_Listener el)
{
	object_event_listener_remove(o, type, el);
}

EAPI Event * event_new(void)
{

}

EAPI Event_Mutation * event_mutation_new(Object *o)
{
	Event_Mutation *evt;
}
