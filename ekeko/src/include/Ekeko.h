#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
#include <Enesim.h>
/**
 * @mainpage Ekeko
 * @section intro Introduction
 * The idea behind this library is to code abstract objects, so any other canvas
 * libraries like evas itself can be built on top of this, the engine + objects (both)
 * must be coded on the upper library, this is just usefullnes without it.
 * Use cases:
 * 1. Filter Objects:
 * The filter object should receive the rectangles where to draw
 * get the pixels from the canvas surface and then filter them on its own surface for later bliting
 * or directly on the target surface.
 * 
 * 2. Double Buffered Canvas:
 * The canvas should have a surface attached i.e the back buffer and another surface on client side
 * to receive areas changed after the process of all objects on the canvas, i.e keep the areas that
 * changed buffered and then blit them from the backbuffer to the target surface. 
 * 
 * 3. Mirror objects, i.e objects that resembles other objects
 * There might be some kind of callback definitions whenever an object changes, something like object
 * listeners. 
 * 
 * @todo
 * + Add support for group of objects
 * + Add support for clip rectangles
 * + Maybe make the subcanvas part of ekeko itslef?
 * + As we have object events, why dont canvas events?
 *  
 * @file
 * @brief Ekeko API
 * @defgroup Ekeko_Group API
 * @{
 * 
 */

/**
 * @defgroup Ekeko_Core_Group Core
 */
typedef enum
{
	EKEKO_ERROR_NONE,
} Ekeko_Error;

extern Ekeko_Error ekeko_err; /**< */

EAPI char * ekeko_error_to_str(unsigned int err);

/**
 * @}
 * @defgroup Ekeko_Canvas_Group Canvas
 * @{ 
 */

typedef struct _Ekeko_Canvas Ekeko_Canvas; /**< Opaque handler */

enum
{
	EKEKO_TILER_SPLITTER,
	EKEKO_TILER_TILEBUF,
	EKEKO_TILERS
};

typedef struct _Ekeko_Canvas_Class
{
	int (*flush)(void *data, Eina_Rectangle *r);
	/* what about the idle? */
	/* canvas_changed -> if some object has changed, then the canvas has changed, we could notify upward that so we can call
	 * the canvas_process
	 **/
} Ekeko_Canvas_Class;

EAPI Ekeko_Canvas * ekeko_canvas_new(Ekeko_Canvas_Class *cclass, void *cdata, int type, int w, int h);
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI void ekeko_canvas_process(Ekeko_Canvas *c);
EAPI void ekeko_canvas_geometry_get(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI void * ekeko_canvas_class_data_get(Ekeko_Canvas *c); 

/**
 * @}
 * @defgroup Ekeko_Object_Group Object
 * @{ 
 */

typedef struct _Ekeko_Object Ekeko_Object;

typedef struct _Ekeko_Object_Class
{
	/* name of the object class */
	const char name[256];
	/* object is freed */
	void (*free)(void *data); 
	/* once the object is going to be processed, first this is called */
	void (*pre_process)(void *data);
	/* the actual process has to be done here receiving the rectangle, FIXME maybe return a value to stop checking this object? */
	void (*process)(void *data, Eina_Rectangle *r);
	/* after the process this is called */
	void (*post_process)(void *data);
	/* return a list of rectangles in case something inside the object has
	 * changed */
	//Ekeko_Rectangle * (*state_changed)(void *data);
} Ekeko_Object_Class;


EAPI Ekeko_Object * ekeko_object_add(Ekeko_Canvas *c, Ekeko_Object_Class *oclass, void *cdata);
EAPI void ekeko_object_move(Ekeko_Object *o, int x, int y);
EAPI void ekeko_object_show(Ekeko_Object *o);
EAPI void ekeko_object_hide(Ekeko_Object *o);
EAPI Eina_Bool ekeko_object_is_visible(Ekeko_Object *o);
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h);
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void * ekeko_object_class_data_get(Ekeko_Object *o);
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o);
EAPI void ekeko_object_geometry_get(Ekeko_Object *o, Eina_Rectangle *r);

/** @} */

/**
 * @}
 * @defgroup Ekeko_Input_Group Input
 * @{
 */
typedef struct _Ekeko_Input Ekeko_Input;

typedef enum _Ekeko_Button_Flags
{
	EKEKO_BUTTON_NONE = 0, /**< No extra mouse button data */
	EKEKO_BUTTON_DOUBLE_CLICK = (1 << 0), /**< This mouse button press was the 2nd press of a double click */
	EKEKO_BUTTON_TRIPLE_CLICK = (1 << 1) /**< This mouse button press was the 3rd press of a triple click */
} Ekeko_Button_Flags; /**< Flags for Mouse Button events */


/* TODO the final parameter (data) can be replaced? i think there's no need
 * for it
 */
EAPI Ekeko_Input * ekeko_input_new(Ekeko_Canvas *c);
EAPI ekeko_input_feed_mouse_down(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i, int b, Ekeko_Button_Flags flags, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, int x, int y, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i, unsigned int timestamp);
EAPI void ekeko_input_feed_mouse_wheel(Ekeko_Input *i, int direction, int z, unsigned int timestamp);
EAPI void ekeko_input_feed_key_down(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp);
EAPI void ekeko_input_feed_key_up(Ekeko_Input *i, const char *keyname, const char *key, const char *string, const char *compose, unsigned int timestamp);
EAPI void ekeko_input_feed_hold(Ekeko_Input *i, int hold, unsigned int timestamp); 

/**
 * @}
 * @defgroup Ekeko_Event_Group Event
 * @{
 */
/* TODO rename all of this as ekeko_object_events */
typedef enum _Ekeko_Event_Type
{
	EKEKO_EVENT_MOUSE_IN, /**< Mouse In Event */
	EKEKO_EVENT_MOUSE_OUT, /**< Mouse Out Event */
	EKEKO_EVENT_MOUSE_DOWN, /**< Mouse Button Down Event */
	EKEKO_EVENT_MOUSE_UP, /**< Mouse Button Up Event */
	EKEKO_EVENT_MOUSE_MOVE, /**< Mouse Move Event */
	EKEKO_EVENT_MOUSE_WHEEL, /**< Mouse Wheel Event */
	EKEKO_EVENT_FREE, /**< Object Being Freed (Called after Del) */
	EKEKO_EVENT_KEY_DOWN, /**< Key Press Event */	
	EKEKO_EVENT_KEY_UP, /**< Key Release Event */
	EKEKO_EVENT_FOCUS_IN, /**< Focus In Event */
	EKEKO_EVENT_FOCUS_OUT, /**< Focus Out Event */
	EKEKO_EVENT_SHOW, /**< Show Event */
	EKEKO_EVENT_HIDE, /**< Hide Event */
	EKEKO_EVENT_MOVE, /**< Move Event */
	EKEKO_EVENT_RESIZE, /**< Resize Event */
	EKEKO_EVENT_RESTACK, /**< Restack Event */
	EKEKO_EVENT_DEL, /**< Object Being Deleted (called before Free) */
	EKEKO_EVENT_HOLD, /**< Events go on/off hold */
	EKEKO_EVENTS,
} Ekeko_Event_Type; /**< The type of event to trigger the callback */

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

typedef struct _Ekeko_Event
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
} Ekeko_Event;

typedef void (*Ekeko_Event_Cb)(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data);
EAPI void ekeko_object_event_callback_add(Ekeko_Object *o, Ekeko_Event_Type etype, Ekeko_Event_Cb cb, void *data);
/** 
 * @}
 */
#endif /*EKEKO_H_*/
