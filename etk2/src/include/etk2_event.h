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
	char *prop;
} Event_Mutation;

typedef void (*Event_Listener)(Event *);

#endif /* ETK2_EVENT_H_ */
