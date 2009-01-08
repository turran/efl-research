/*
 * etk2_event.h
 *
 *  Created on: 07-ene-2009
 *      Author: jl
 */
#ifndef ETK2_EVENT_H_
#define ETK2_EVENT_H_

typedef struct _Event
{
	const char *type;
	Object *target;
} Event;

typedef struct _Event_Mutation
{
	Event event;
	Object *related;
	Type_Property_Value prev;
	Type_Property_Value curr;
	const char *prop;
} Event_Mutation;

typedef void (*Event_Listener)(Event *);

EAPI void event_listener_add(Object *o, const char *type, Event_Listener el);
EAPI void event_listener_remove(Object *o, const char *type, Event_Listener el);
EAPI void event_dispatch(Event *e);
EAPI void event_init(Event *e, const char *type, Object *o);


#endif /* ETK2_EVENT_H_ */
