/*
 * etk2_canvas.h
 *
 *  Created on: 12-ene-2009
 *      Author: jl
 */
#ifndef ETK2_CANVAS_H_
#define ETK2_CANVAS_H_

typedef struct _Canvas_Private Canvas_Private;

struct _Canvas
{
	Renderable parent;
	/* inform the canvas that an area must be flushed
	 * returns EINA_TRUE if the whole canvas has been flushed
	 * or EINA_FALSE if only the needed rectangle
	 */
	Eina_Bool (*flush)(Canvas *, Eina_Rectangle *);
	Canvas_Private *private;
};

Type *canvas_type_get(void);
EAPI void canvas_size_set(Canvas *c, int w, int h);
EAPI void canvas_damage_add(Canvas *c, Eina_Rectangle *r);

#endif /* ETK2_CANVAS_H_ */
