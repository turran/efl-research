#ifndef EKEKO_OBJECT_H_
#define EKEKO_OBJECT_H_

typedef struct _Ekeko_Object_Cb Ekeko_Object_Cb;

/* every object should receive the area or areas to draw based on their
 * boundings
 */

/* do we need this state fixed with priv data on each or better make
 * callbacks like _state_has_changed and store both states as void *
 */
typedef struct _Ekeko_Object_State
{
	Eina_Rectangle geometry;
	Eina_Bool visible;
	void *data;
} Ekeko_Object_State;

struct _Ekeko_Object_Cb
{
	Eina_Inlist list;
	Ekeko_Event_Cb func;
	void *data;
};

struct _Ekeko_Object
{
	Eina_Inlist list;
	Ekeko_Object_Class *oclass;
	Ekeko_Canvas *canvas;

	Ekeko_Object_State curr;
	Ekeko_Object_State prev;
	Ekeko_Object_Cb *callbacks[EKEKO_EVENTS];
	Eina_Bool delete_me;
	Eina_Bool changed;
	void *cdata;
};

Eina_Bool ekeko_object_is_inside(Ekeko_Object *o, Ekeko_Rectangle *r, Eina_Rectangle *drect);
void ekeko_object_pre_process(Ekeko_Object *o);
void ekeko_object_process(Ekeko_Object *o, Eina_Rectangle *r);
void ekeko_object_post_process(Ekeko_Object *o);
void ekeko_object_event_callback_call(Ekeko_Object *o, Ekeko_Event_Type type, Ekeko_Event *ev);

#endif
