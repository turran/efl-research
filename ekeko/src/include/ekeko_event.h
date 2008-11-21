#ifndef EKEKO_EVENT_H_
#define EKEKO_EVENT_H_

/**
 * @defgroup Ekeko_Event_Group Event
 * @{
 */
/* TODO rename all of this as ekeko_object_events */
typedef enum _Ekeko_Event_Type_Old
{
	EKEKO_EVENT_MOUSE_IN, /**< Mouse In Event */
	EKEKO_EVENT_MOUSE_OUT, /**< Mouse Out Event */
	EKEKO_EVENT_MOUSE_DOWN, /**< Mouse Button Down Event */
	EKEKO_EVENT_MOUSE_UP, /**< Mouse Button Up Event */
	EKEKO_EVENT_MOUSE_MOVE, /**< Mouse Move Event */
	EKEKO_EVENT_MOUSE_WHEEL, /**< Mouse Wheel Event */
	EKEKO_EVENT_FREE, /**< Renderable Being Freed (Called after Del) */
	EKEKO_EVENT_KEY_DOWN, /**< Key Press Event */	
	EKEKO_EVENT_KEY_UP, /**< Key Release Event */
	EKEKO_EVENT_FOCUS_IN, /**< Focus In Event */
	EKEKO_EVENT_FOCUS_OUT, /**< Focus Out Event */
	EKEKO_EVENT_SHOW, /**< Show Event */
	EKEKO_EVENT_HIDE, /**< Hide Event */
	EKEKO_EVENT_MOVE, /**< Move Event */
	EKEKO_EVENT_RESIZE, /**< Resize Event */
	EKEKO_EVENT_RESTACK, /**< Restack Event */
	EKEKO_EVENT_USER_CHANGE, /**< User provided change */
	EKEKO_EVENT_DEL, /**< Renderable Being Deleted (called before Free) */
	EKEKO_EVENT_HOLD, /**< Events go on/off hold */
	EKEKO_EVENTS,
} Ekeko_Event_Type_Old; /**< The type of event to trigger the callback */

struct _Ekeko_Event_Mouse_Down /** Mouse button press event */
{
	int button; /**< Mouse button number that went down (1 - 32) */
	unsigned int x, y;
	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;

	Ekeko_Button_Flags flags;
	unsigned int timestamp;
};

struct _Ekeko_Event_Mouse_Up /** Mouse butotn relase event */
{
	int button; /**< Mouse button number that was raised (1 - 32) */
	unsigned int x, y;
	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;

	Ekeko_Button_Flags flags;
	unsigned int timestamp;
};

struct _Ekeko_Event_Mouse_In /** Mouse enter event */
{
	int buttons; /**< Button pressed mask, Bits set to 1 are buttons currently pressed (bit 0 = mouse button 1, bit 1 = mouse button 2 etc.) */
	unsigned int x, y;
	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;
	unsigned int timestamp;
};

struct _Ekeko_Event_Mouse_Out /** Mouse leave event */
{
	int buttons; /**< Button pressed mask, Bits set to 1 are buttons currently pressed (bit 0 = mouse button 1, bit 1 = mouse button 2 etc.) */
	unsigned int x, y;
	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;
	unsigned int timestamp;
};

struct _Ekeko_Event_Mouse_Move /** Mouse button down event */
{
	int buttons; /**< Button pressed mask, Bits set to 1 are buttons currently pressed (bit 0 = mouse button 1, bit 1 = mouse button 2 etc.) */
	struct {
		unsigned int x, y;
	} cur, prev;
	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;
	unsigned int timestamp;
};

struct _Ekeko_Event_Mouse_Wheel /** Wheel event */
{
	int direction; /* 0 = default up/down wheel FIXME: more wheel types */
	int z; /* ...,-2,-1 = down, 1,2,... = up */

	unsigned int x, y;

	void *data;
	//Ekeko_Modifier *modifiers;
	//Ekeko_Lock *locks;
	unsigned int timestamp;
};

typedef struct _Ekeko_Event_Mouse_Down Ekeko_Event_Mouse_Down; /**< Event structure for #EKEKO_EVENT_MOUSE_DOWN event callbacks */
typedef struct _Ekeko_Event_Mouse_Up   Ekeko_Event_Mouse_Up; /**< Event structure for #EKEKO_EVENT_MOUSE_UP event callbacks */
typedef struct _Ekeko_Event_Mouse_In   Ekeko_Event_Mouse_In; /**< Event structure for #EKEKO_EVENT_MOUSE_IN event callbacks */
typedef struct _Ekeko_Event_Mouse_Out  Ekeko_Event_Mouse_Out; /**< Event structure for #EKEKO_EVENT_MOUSE_OUT event callbacks */
typedef struct _Ekeko_Event_Mouse_Move Ekeko_Event_Mouse_Move; /**< Event structure for #EKEKO_EVENT_MOUSE_MOVE event callbacks */
typedef struct _Ekeko_Event_Mouse_Wheel Ekeko_Event_Mouse_Wheel; /**< Event structure for #EKEKO_EVENT_MOUSE_WHEEL event callbacks */

typedef struct _Ekeko_Event_old
{
	Ekeko_Input *input; /* TODO is it needed for every event?? */
	union
	{
		Ekeko_Event_Mouse_Down mouse_down;
		Ekeko_Event_Mouse_Up mouse_up;
		Ekeko_Event_Mouse_In mouse_in;
		Ekeko_Event_Mouse_Out mouse_out;
		Ekeko_Event_Mouse_Move mouse_move; 
		Ekeko_Event_Mouse_Wheel mouse_wheel;
	} data;
} Ekeko_Event_old;
#if 0
typedef void (*Ekeko_Event_Cb)(Ekeko_Canvas *c, Ekeko_Renderable *o, Ekeko_Event *ev, void *data);
EAPI void ekeko_object_event_callback_add(Ekeko_Renderable *o, Ekeko_Event_Type etype, Ekeko_Event_Cb cb, void *data);
#endif

typedef void (*Ekeko_Event_Listener)(Ekeko_Event *);

typedef enum _Ekeko_Event_Phase
{
	EKEKO_EVENT_PHASE_CAPTURING,
	EKEKO_EVENT_PHASE_AT_TARGET,
	EKEKO_EVENT_PHASE_BUBBLING,
} Ekeko_Event_Phase;

// Introduced in DOM Level 2:
struct _Ekeko_Event
{
	Ekeko_Event_Phase phase;
	const char *type;
	Ekeko_Node *target;
	Ekeko_Node *current_target;
#if 0	
  readonly attribute DOMString        type;
  readonly attribute EventTarget      target;
  readonly attribute EventTarget      currentTarget;
  readonly attribute unsigned short   eventPhase;
  readonly attribute boolean          bubbles;
  readonly attribute boolean          cancelable;
  readonly attribute DOMTimeStamp     timeStamp;
  void               stopPropagation();
  void               preventDefault();
  void               initEvent(in DOMString eventTypeArg, 
                               in boolean canBubbleArg, 
                               in boolean cancelableArg);
#endif
};

EAPI void ekeko_event_init(Ekeko_Event *e, const char *type, Eina_Bool can_bubble,
		Eina_Bool can_cancel);

typedef enum _Ekeko_Event_Mutation_Change
{
	EKEKO_EVENT_MUTATION_MODIFCATION = 1,
	EKEKO_EVENT_MUTATION_ADDITION,
	EKEKO_EVENT_MUTATION_REMOVAL,
} Ekeko_Event_Mutation_Change;


typedef struct _Ekeko_Event_Mutation
{
	Ekeko_Event event;
	Ekeko_Event_Mutation_Change change;
	Ekeko_Node *related; /* node where the mutation has happened */
	Ekeko_Value *prev;
	Ekeko_Value *curr;
	char *attr;
} Ekeko_Event_Mutation;

EAPI void ekeko_event_mutation_init(Ekeko_Event_Mutation *e, const char *type,
		Eina_Bool bubble, Eina_Bool cancelable, Ekeko_Node *rel,
		Ekeko_Value *prev, Ekeko_Value *curr, const char *attr,
		Ekeko_Event_Mutation_Change change);

typedef enum _Ekeko_Event_Process_Phase
{
	EKEKO_EVENT_PROCESS_PHASE_PRE,
	EKEKO_EVENT_PROCESS_PHASE,
	EKEKO_EVENT_PROCESS_PHASE_POST,
} Ekeko_Event_Process_Phase;

typedef struct _Ekeko_Event_Process
{
	Ekeko_Event event;
	Ekeko_Event_Process_Phase phase;
	Ekeko_Node *related;
} Ekeko_Event_Process;


/** 
* @}
*/

#endif /*EKEKO_EVENT_H_*/
