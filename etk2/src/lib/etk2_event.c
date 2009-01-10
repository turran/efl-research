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
		const Object *rel, const Property *prop, Value *prev, Value *curr)
{
	event_init((Event *)em, type, o);
	em->related = rel;
	em->prev = prev;
	em->curr = curr;
	if (prop)
	{
		em->prop = property_name_get(prop);
		em->prop_id = property_id_get(prop);
	}
}
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

EAPI void event_dispatch(Event *e)
{
	object_event_dispatch(e->target, e);
}

EAPI void event_init(Event *e, const char *type, const Object *o)
{
	e->target = o;
	e->type = type;
}
#if 0
EAPI void event_property_int_changed(Event *e, const char *type,
		const Object *o, int prev, int curr)
{

}
#endif

