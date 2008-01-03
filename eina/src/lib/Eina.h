#ifndef EINA_H_
#define EINA_H_

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/* 
 * The idea behind this library is to code abstract objects, so any other canvas
 * libraries like evas itself can be built on top of this, the engine + objects (both)
 * must be coded on the upper library, this is just usefullnes without it.
 */

typedef struct _Eina_Canvas Eina_Canvas;
typedef struct _Eina_Object Eina_Object;

/* possible canvas api */
EAPI Eina_Canvas * eina_canvas_new(int w, int h);
EAPI void eina_canvas_damage_add(Eina_Canvas *c, int x, int y, int w, int h);
EAPI void eina_canvas_obscure_add(Eina_Canvas *c, int x, int y, int w, int h);
EAPI void eina_canvas_obscure_del(Eina_Canvas *c, int x, int y, int w, int h);
EAPI void eina_canvas_process(Eina_Canvas *c);

/* possible object api */
EAPI Eina_Object * eina_object_add(Eina_Canvas *c);
EAPI void eina_object_move(Eina_Object *o, int x, int y);
EAPI void eina_object_show(Eina_Object *o);
EAPI void eina_object_hide(Eina_Object *o);
EAPI void eina_object_resize(Eina_Object *o, int w, int h);
EAPI void eina_object_stack_above(Eina_Object *o, Eina_Object *object_rel);
EAPI void eina_object_stack_below(Eina_Object *o, Eina_Object *object_rel);
/* 
 * invalidate all the object, what about invalidating only a part of it? pass a
 * list of rectangles?
 * what about objects that obscure completely whats behind, so we skip the object
 * list below, use some object property? 
 */
EAPI void eina_object_invalidate(Eina_Object *o);

/* possible event api */
/* interceptors */

#endif /*EINA_H_*/
