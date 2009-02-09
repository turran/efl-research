/*
 * etk2_shape.h
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */

#ifndef ETK2_SHAPE_H_
#define ETK2_SHAPE_H_

typedef struct _Etk_Shape_Private Etk_Shape_Private;
struct _Etk_Shape
{
	Ekeko_Renderable parent;
	Etk_Shape_Private *private;
};

EAPI Ekeko_Type *etk_shape_type_get(void);
EAPI void etk_shape_color_set(Etk_Shape *s, int color);

#endif /* ETK2_SHAPE_H_ */
