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
	const Object *target;
} Event;

#define EVENT_PROP_MODIFY "PropModify"
#define EVENT_OBJECT_APPEND "ObjectAppend"
#define EVENT_OBJECT_REMOVE "ObjectRemove"

typedef struct _Event_Mutation
{
	Event event;
	const Object *related;
	Value *prev;
	Value *curr;
	const char *prop;
	Property_Id prop_id;
} Event_Mutation;

typedef struct _Event_Property_Int
{
	Event_Mutation event;
	int prev;
	int curr;
} Event_Property_Int;

typedef struct _Event_Property_String
{
	Event_Mutation event;
	char *prev;
	char *curr;
} Event_Property_String;

typedef void (*Event_Listener)(Event *);

EAPI void event_listener_add(Object *o, const char *type, Event_Listener el);
EAPI void event_listener_remove(Object *o, const char *type, Event_Listener el);
EAPI void event_dispatch(Event *e);
EAPI void event_init(Event *e, const char *type, const Object *o);


#endif /* ETK2_EVENT_H_ */
