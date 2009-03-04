/*
 * etk2_event.h
 *
 *  Created on: 07-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_EVENT_H_
#define EKEKO_EVENT_H_

typedef struct _Event
{
	const char *type;
	const Ekeko_Object *target;
	Eina_Bool bubbles;
} Event;

#define EVENT_PROP_MODIFY "PropModify"
#define EVENT_PARENT_SET "ParentSet"
/* The EVENT_PARENT_SET event gets triggered whenever an object's parent
 * has been set (Event_Mutation).
 * target: Child
 * related: Parent
 */
#define EVENT_OBJECT_APPEND "ObjectAppend"
/* The EVENT_OBJECT_APPEND event gets triggered whenever an object has been
 * appended to another object (Event_Mutation).
 * FIXME right now is the inverse
 * target: Parent
 * related: Child
 */
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
	const Ekeko_Object *related; /* parent in case of child_append/remove */
	Ekeko_Value *prev; /* previous value */
	Ekeko_Value *curr; /* current value */
	const char *prop; /* property name */
	Property_Id prop_id; /* property id */
} Event_Mutation;

typedef struct _Event_Ui
{
	Event event;
	const Ekeko_Object *related;
	const Ekeko_Input *i;
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

typedef void (*Event_Listener)(const Ekeko_Object *, Event *, void * data);

EAPI void ekeko_event_listener_add(Ekeko_Object *o, const char *type, Event_Listener el, Eina_Bool bubble, void *data);
EAPI void ekeko_event_listener_remove(Ekeko_Object *o, const char *type, Event_Listener el, Eina_Bool bubble, void *data);
EAPI void ekeko_event_dispatch(Event *e);
EAPI void ekeko_event_init(Event *e, const char *type, const Ekeko_Object *o, Eina_Bool bubbles);


#endif /* ETK2_EVENT_H_ */
