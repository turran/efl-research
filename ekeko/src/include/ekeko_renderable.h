#ifndef EKEKO_RENDERABLE_H_
#define EKEKO_RENDERABLE_H_

/*
 * @defgroup Ekeko_Renderable_Group Renderable
 * @{ 
 */

typedef struct _Ekeko_Renderable_Class
{
	/* name of the renderable class */
	const char name[256];
	/* renderable is freed */
	void (*free)(void *data); 
	/* once the renderable is going to be processed, first this is called */
	void (*pre_process)(void *data);
	/* the actual process has to be done here receiving the rectangle, FIXME maybe return a value to stop checking this renderable? */
	void (*process)(void *data, Eina_Rectangle *r);
	/* after the process this is called */
	void (*post_process)(void *data);
	/* the inside check is calculated against the bounding box, this function
	 * determines if the renderable is really inside a giving rectangle
	 */
	Eina_Bool (*is_inside)(void *data, Eina_Rectangle *r);
	/* return a list of rectangles in case something inside the renderable has
	 * changed */
	//Ekeko_Rectangle * (*state_changed)(void *data);
} Ekeko_Renderable_Class;


#if 0
/**
 * TODO
 * should we make this renderable flags?
 */
typedef enum _Ekeko_Renderable_Flag
{
	EKEKO_OBJECT_FLAG_HIDDEN_RECEIVE = (1 << 0),
} Ekeko_Renderable_Flag;

typedef Eina_Bool (*Ekeko_Renderable_Cmp_Func)(Ekeko_Renderable *o, void *data);
EAPI Ekeko_Renderable * ekeko_renderable_add(Ekeko_Canvas *c, Ekeko_Renderable_Class *oclass, void *cdata);
EAPI void ekeko_renderable_move(Ekeko_Renderable *o, int x, int y);
EAPI void ekeko_renderable_show(Ekeko_Renderable *o);
EAPI void ekeko_renderable_hide(Ekeko_Renderable *o);
EAPI Eina_Bool ekeko_renderable_is_visible(Ekeko_Renderable *o);
EAPI void ekeko_renderable_resize(Ekeko_Renderable *o, int w, int h);
EAPI void ekeko_renderable_stack_above(Ekeko_Renderable *o, Ekeko_Renderable *renderable_rel);
EAPI void ekeko_renderable_stack_below(Ekeko_Renderable *o, Ekeko_Renderable *renderable_rel);
EAPI void * ekeko_renderable_class_data_get(Ekeko_Renderable *o);
EAPI Ekeko_Canvas * ekeko_renderable_canvas_get(Ekeko_Renderable *o);
EAPI void ekeko_renderable_geometry_get(Ekeko_Renderable *o, Eina_Rectangle *r);
EAPI Ekeko_Renderable * ekeko_renderable_rel_get_down(Ekeko_Renderable *rel, Ekeko_Renderable_Cmp_Func cmp, void *data);
EAPI Ekeko_Renderable * ekeko_renderable_rel_get_up(Ekeko_Renderable *rel, Ekeko_Renderable_Cmp_Func cmp, void *data);
EAPI Eina_Bool ekeko_renderable_is_inside(Ekeko_Renderable *o, Eina_Rectangle *r);
EAPI Eina_Bool ekeko_renderable_geometry_is_inside(Ekeko_Renderable *o, Eina_Rectangle *r);
#endif
/** @} */


#endif /*EKEKO_RENDERABLE_H_*/
