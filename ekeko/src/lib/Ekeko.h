#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
#include <Enesim.h>
/* 
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
 */

typedef struct _Ekeko_Canvas Ekeko_Canvas;
typedef struct _Ekeko_Object Ekeko_Object;
typedef struct _Ekeko_Rectangle Ekeko_Rectangle;

struct _Ekeko_Rectangle
{
	Eina_Inlist list;
	Enesim_Rectangle r;
};

enum
{
	EKEKO_TILER_SPLITTER,
	EKEKO_TILERS
};

/* possible canvas api */
EAPI Ekeko_Canvas * ekeko_canvas_new(int type, int w, int h);
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, Enesim_Rectangle *r);
EAPI void ekeko_canvas_process(Ekeko_Canvas *c);
EAPI void ekeko_canvas_data_set(Ekeko_Canvas *c, void *data);
EAPI void * ekeko_canvas_data_get(Ekeko_Canvas *c);

/* possible object api */
typedef struct _Ekeko_Object_Class
{
	/* object is created after object_new */
	void (*create)(Ekeko_Object *o); 
	/* object is freed */
	void (*free)(void *data); 
	/* once the object is going to be processed, first this is called */
	void (*pre_process)(void *data);
	/* the actual process has to be done here receiving the rectangle */
	void (*process)(void *data, Enesim_Rectangle *r);
	/* after the process this is called */
	void (*post_process)(void *data);
	/* return a list of rectangles in case something inside the object has
	 * changed */
	//Ekeko_Rectangle * (*state_changed)(void *data);
} Ekeko_Object_Class;

EAPI Ekeko_Object * ekeko_object_add(Ekeko_Canvas *c, Ekeko_Object_Class *oclass);
EAPI void ekeko_object_move(Ekeko_Object *o, int x, int y);
EAPI void ekeko_object_show(Ekeko_Object *o);
EAPI void ekeko_object_hide(Ekeko_Object *o);
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h);
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void ekeko_object_data_set(Ekeko_Object *o, void *data);
EAPI void * ekeko_object_data_get(Ekeko_Object *o);
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o);
/* 
 * invalidate all the object, what about invalidating only a part of it? pass a
 * list of rectangles?
 * what about objects that obscure completely whats behind, so we skip the object
 * list below, use some object property? 
 */
EAPI void ekeko_object_invalidate(Ekeko_Object *o);

/* possible event api */
/* interceptors */

#endif /*EKEKO_H_*/
