/*
 * etk2_canvas.h
 *
 *  Created on: 12-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_CANVAS_H_
#define EKEKO_CANVAS_H_

typedef struct _Ekeko_Canvas_Private Ekeko_Canvas_Private;

struct _Ekeko_Canvas
{
	Ekeko_Renderable parent;
	/* inform the canvas that an area must be flushed
	 * returns EINA_TRUE if the whole canvas has been flushed
	 * or EINA_FALSE if only the needed rectangle
	 */
	Eina_Bool (*flush)(Ekeko_Canvas *, Eina_Rectangle *);
	Ekeko_Canvas_Private *private;
};

Ekeko_Type *ekeko_canvas_type_get(void);
EAPI void ekeko_canvas_size_set(Ekeko_Canvas *c, int w, int h);
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Eina_Rectangle *r);
EAPI Ekeko_Input * ekeko_canvas_input_new(Ekeko_Canvas *c);
EAPI Ekeko_Renderable * ekeko_canvas_renderable_get_at_coord(Ekeko_Canvas *c, unsigned int x, unsigned int y);

#endif /* ETK2_CANVAS_H_ */
