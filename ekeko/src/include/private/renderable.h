#ifndef RENDERABLE_H_
#define RENDERABLE_H_

typedef struct _Ekeko_Renderable_Cb Ekeko_Renderable_Cb;

/* every renderable should receive the area or areas to draw based on their
 * boundings
 */

/* do we need this state fixed with priv data on each or better make
 * callbacks like _state_has_changed and store both states as void *
 */
typedef struct _Ekeko_Renderable_State
{
	Eina_Rectangle geometry;
	Eina_Bool visible;
	void *data;
} Ekeko_Renderable_State;

struct _Ekeko_Renderable_Cb
{
	EINA_INLIST;
	Ekeko_Event_Cb func;
	void *data;
};

struct _Ekeko_Renderable
{
	EINA_INLIST;
	Ekeko_Renderable_Class *oclass;
	Ekeko_Canvas *canvas;

	Ekeko_Renderable_State curr;
	Ekeko_Renderable_State prev;
	Eina_Inlist *callbacks[EKEKO_EVENTS];
	Eina_Bool delete_me;
	Eina_Bool changed;
	Eina_Bool valid;
	/* TODO
	 * add a way to pass the event to the renderables behind
	 */
	void *cdata;
};

Eina_Bool ekeko_renderable_intersection_get(Ekeko_Renderable *o, Ekeko_Rectangle *r, Eina_Rectangle *drect);
void ekeko_renderable_pre_process(Ekeko_Renderable *o);
void ekeko_renderable_process(Ekeko_Renderable *o, Eina_Rectangle *r);
void ekeko_renderable_post_process(Ekeko_Renderable *o);
void ekeko_renderable_event_callback_call(Ekeko_Renderable *o, Ekeko_Event_Type type, Ekeko_Event *ev);
void ekeko_renderable_validate(Ekeko_Renderable *o);
void ekeko_renderable_invalidate(Ekeko_Renderable *o);

#endif
