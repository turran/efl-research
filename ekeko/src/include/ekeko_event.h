/*
 * etk2_event.h
 *
 *  Created on: 07-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_EVENT_H_
#define EKEKO_EVENT_H_

typedef struct _Ekeko_Event
{
	const char *type;
	const Ekeko_Object *target;
	Eina_Bool bubbles;
} Ekeko_Event;

#define EKEKO_EVENT_PROP_MODIFY "PropModify"
#define EKEKO_EVENT_PARENT_SET "ParentSet"
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
#define EKEKO_EVENT_OBJECT_REMOVE "ObjectRemove"
#define EKEKO_EVENT_OBJECT_PROCESS "ObjectProcess"
#define EKEKO_EVENT_UI_MOUSE_MOVE "mousemove"
#define EKEKO_EVENT_UI_MOUSE_IN "mousein"
#define EKEKO_EVENT_UI_MOUSE_OUT "mouseout"
#define EKEKO_EVENT_UI_MOUSE_DOWN "mousedown"
#define EKEKO_EVENT_UI_MOUSE_UP "mouseup"
#define EKEKO_EVENT_UI_MOUSE_CLICK "click"

typedef enum _Event_Mutation_State
{
	EVENT_MUTATION_STATE_PRE,
	EVENT_MUTATION_STATE_CURR,
	EVENT_MUTATION_STATE_POST,
} Ekeko_Event_Mutation_State;

typedef struct _Event_Mutation
{
	Ekeko_Event event;
	Ekeko_Event_Mutation_State state; /* pre/post (async) curr (async/sync) */
	const Ekeko_Object *related; /* parent in case of child_append/remove */
	Ekeko_Value *prev; /* previous value */
	Ekeko_Value *curr; /* current value */
	const char *prop; /* property name */
	Ekeko_Property_Id prop_id; /* property id */
} Ekeko_Event_Mutation;

typedef struct _Event_Ui
{
	Ekeko_Event event;
	const Ekeko_Object *related;
	const Ekeko_Input *i;
	/* TODO for now this isnt needed but whenever the keyboard event
	 * is finished we might find duplicate things on the mouse
	 */
} Ekeko_Event_Ui;

typedef struct _Event_Mouse
{
	Ekeko_Event_Ui ui;
	/* screen */
	struct
	{
		unsigned int x;
		unsigned int y;
	} screen;
} Ekeko_Event_Mouse;

typedef void (*Event_Listener)(const Ekeko_Object *, Ekeko_Event *, void * data);

EAPI void ekeko_event_listener_add(Ekeko_Object *o, const char *type, Event_Listener el, Eina_Bool bubble, void *data);
EAPI void ekeko_event_listener_remove(Ekeko_Object *o, const char *type, Event_Listener el, Eina_Bool bubble, void *data);
EAPI void ekeko_event_dispatch(Ekeko_Event *e);
EAPI void ekeko_event_init(Ekeko_Event *e, const char *type, const Ekeko_Object *o, Eina_Bool bubbles);


#endif /* ETK2_EVENT_H_ */
