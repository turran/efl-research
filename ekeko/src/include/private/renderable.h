#ifndef RENDERABLE_H_
#define RENDERABLE_H_

void ekeko_renderable_render(Ekeko_Renderable *r, Ekeko_Element *parent, Eina_Rectangle *er);
void ekeko_renderable_render_all(Ekeko_Element *c, Eina_Inlist *renderables, Eina_Rectangle *er);
Eina_Inlist * ekeko_renderable_add(Eina_Inlist *renderables, Ekeko_Renderable *r);

#if 0
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
#if 0
	Ekeko_Event_Cb func;
#endif
	void *data;
};
Eina_Bool ekeko_renderable_intersection_get(Ekeko_Renderable *o, Ekeko_Rectangle *r, Eina_Rectangle *drect);
void ekeko_renderable_pre_process(Ekeko_Renderable *o);
void ekeko_renderable_process(Ekeko_Renderable *o, Eina_Rectangle *r);
void ekeko_renderable_post_process(Ekeko_Renderable *o);
//void ekeko_renderable_event_callback_call(Ekeko_Renderable *o, Ekeko_Event_Type type, Ekeko_Event *ev);
void ekeko_renderable_validate(Ekeko_Renderable *o);
void ekeko_renderable_invalidate(Ekeko_Renderable *o);
#endif
#endif
