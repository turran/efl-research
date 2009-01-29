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
	Eina_Bool bubbles;
} Event;

#define EVENT_PROP_MODIFY "PropModify"
#define EVENT_OBJECT_APPEND "ObjectAppend"
#define EVENT_OBJECT_REMOVE "ObjectRemove"
#define EVENT_OBJECT_PROCESS "ObjectProcess"
#define EVENT_UI_MOUSE_MOVE "mousemove"
#define EVENT_UI_MOUSE_IN "mousein"
#define EVENT_UI_MOUSE_OUT "mouseout"
#define EVENT_UI_MOUSE_DOWN "mousedown"
#define EVENT_UI_MOUSE_UP "mouseup"
#define EVENT_UI_MOUSE_CLICK "click"

typedef enum _Event_Mutation_State
{
	EVENT_MUTATION_STATE_PRE,
	EVENT_MUTATION_STATE_CURR,
	EVENT_MUTATION_STATE_POST,
} Event_Mutation_State;

typedef struct _Event_Mutation
{
	Event event;
	Event_Mutation_State state; /* pre/post (async) curr (async/sync) */
	const Object *related; /* parent in case of child_append/remove */
	Value *prev; /* previous value */
	Value *curr; /* current value */
	const char *prop; /* property name */
	Property_Id prop_id; /* property id */
} Event_Mutation;

typedef struct _Event_Ui
{
	Event event;
	const Object *related;
	const Input *i;
	/* TODO for now this isnt needed but whenever the keyboard event
	 * is finished we might find duplicate things on the mouse
	 */
} Event_Ui;

typedef struct _Event_Mouse
{
	Event_Ui ui;
	/* screen */
	struct
	{
		unsigned int x;
		unsigned int y;
	} screen;
} Event_Mouse;

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

typedef void (*Event_Listener)(const Object *, Event *);

EAPI void event_listener_add(Object *o, const char *type, Event_Listener el, Eina_Bool bubble);
EAPI void event_listener_remove(Object *o, const char *type, Event_Listener el);
EAPI void event_dispatch(Event *e);
EAPI void event_init(Event *e, const char *type, const Object *o, Eina_Bool bubbles);


#endif /* ETK2_EVENT_H_ */
