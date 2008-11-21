#ifndef EKEKO_CANVAS_H_
#define EKEKO_CANVAS_H_

#define CANVAS_GEOMETRY "geometry"
#define CANVAS_PRIVATE "_canvas"

/* what about the idle? */
/* canvas_changed -> if some object has changed, then the canvas has changed, we could notify upward that so we can call
 * the canvas_process
 */
typedef Eina_Bool (*Ekeko_Canvas_Flush)(Ekeko_Element *e, Eina_Rectangle *r);

EAPI void ekeko_canvas_new(Ekeko_Element *e, Ekeko_Canvas_Flush flush);

EAPI void ekeko_canvas_damage_add(Ekeko_Element *e, Eina_Rectangle *r);
EAPI void ekeko_canvas_obscure_add(Ekeko_Element *e, Eina_Rectangle *r);
EAPI void ekeko_canvas_obscure_del(Ekeko_Element *e, Eina_Rectangle *r);
EAPI void ekeko_canvas_geometry_get(Ekeko_Element *c, unsigned int *w, unsigned int *h);
EAPI void ekeko_canvas_geometry_set(Ekeko_Element *c, unsigned int w, unsigned int h);
//EAPI Ekeko_Renderable * ekeko_canvas_object_from_last_get(Ekeko_Canvas *c, Ekeko_Renderable_Cmp_Func cmp, void *data);
//EAPI Ekeko_Renderable * ekeko_canvas_object_from_first_get(Ekeko_Canvas *c, Ekeko_Renderable_Cmp_Func cmp, void *data);

/**
 * @}
 */


#endif /*EKEKO_CANVAS_H_*/
