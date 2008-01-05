#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
/* 
 * The idea behind this library is to code abstract objects, so any other canvas
 * libraries like evas itself can be built on top of this, the engine + objects (both)
 * must be coded on the upper library, this is just usefullnes without it.
 */

typedef struct _Ekeko_Canvas Ekeko_Canvas;
typedef struct _Ekeko_Object Ekeko_Object;

/* possible canvas api */
EAPI Ekeko_Canvas * ekeko_canvas_new(int w, int h);
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, int x, int y, int w, int h);
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, int x, int y, int w, int h);
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, int x, int y, int w, int h);
EAPI void ekeko_canvas_process(Ekeko_Canvas *c);

/* possible object api */
EAPI Ekeko_Object * ekeko_object_add(Ekeko_Canvas *c);
EAPI void ekeko_object_move(Ekeko_Object *o, int x, int y);
EAPI void ekeko_object_show(Ekeko_Object *o);
EAPI void ekeko_object_hide(Ekeko_Object *o);
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h);
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel);
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel);
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
