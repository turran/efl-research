#ifndef EKEKO_RENDERABLE_H_
#define EKEKO_RENDERABLE_H_

/*
 * @defgroup Ekeko_Object_Group Object
 * @{ 
 */

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
	/* the inside check is calculated against the bounding box, this function
	 * determines if the object is really inside a giving rectangle
	 */
	Eina_Bool (*is_inside)(void *data, Eina_Rectangle *r);
	/* return a list of rectangles in case something inside the object has
	 * changed */
	//Ekeko_Rectangle * (*state_changed)(void *data);
} Ekeko_Object_Class;


#if 0
/**
 * TODO
 * should we make this object flags?
 */
typedef enum _Ekeko_Object_Flag
{
	EKEKO_OBJECT_FLAG_HIDDEN_RECEIVE = (1 << 0),
} Ekeko_Object_Flag;
#endif


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
EAPI Ekeko_Object * ekeko_object_rel_get_down(Ekeko_Object *rel, Ekeko_Object_Cmp_Func cmp, void *data);
EAPI Ekeko_Object * ekeko_object_rel_get_up(Ekeko_Object *rel, Ekeko_Object_Cmp_Func cmp, void *data);
EAPI Eina_Bool ekeko_object_is_inside(Ekeko_Object *o, Eina_Rectangle *r);
EAPI Eina_Bool ekeko_object_geometry_is_inside(Ekeko_Object *o, Eina_Rectangle *r);

/** @} */


#endif /*EKEKO_RENDERABLE_H_*/
