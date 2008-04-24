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
} Ekeko_Error;


extern Ekeko_Error ekeko_err; /**< */


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
	int (*flush)(void *data, Enesim_Rectangle *r);
	/* what about the idle? */
	/* canvas_changed -> if some object has changed, then the canvas has changed, we could notify upward that so we can call
	 * the canvas_process
	 **/
} Ekeko_Canvas_Class;

EAPI Ekeko_Canvas * ekeko_canvas_new(Ekeko_Canvas_Class *cclass, void *cdata, int type, int w, int h);
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_process(Ekeko_Canvas *c);
EAPI void ekeko_canvas_geometry_get(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void * ekeko_canvas_class_data_get(Ekeko_Canvas *c); 

/**
 * @}
 * @defgroup Ekeko_Object_Group Object
 * @{ 
 */

typedef struct _Ekeko_Object Ekeko_Object;

typedef struct _Ekeko_Object_Class
{
	/* object is freed */
	void (*free)(void *data); 
	/* once the object is going to be processed, first this is called */
	void (*pre_process)(void *data);
	/* the actual process has to be done here receiving the rectangle, FIXME maybe return a value to stop checking this object? */
	void (*process)(void *data, Enesim_Rectangle *r);
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
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h);
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void * ekeko_object_class_data_get(Ekeko_Object *o);
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o);
EAPI void ekeko_object_geometry_get(Ekeko_Object *o, Enesim_Rectangle *r);

/** @} */

/* possible event api */
/* interceptors */
/** 
 * @}
 */
#endif /*EKEKO_H_*/
