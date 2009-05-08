/*
 * etk2_shape.h
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */

#ifndef ETK2_SHAPE_H_
#define ETK2_SHAPE_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_SHAPE_COLOR_CHANGED "colorChanged"
#define ETK_SHAPE_ROP_CHANGED "ropChanged"
#define ETK_SHAPE_FILTER_CHANGED "filterChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_SHAPE_COLOR;
extern Property_Id ETK_SHAPE_ROP;
extern Property_Id ETK_SHAPE_FILTER;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Shape_Private Etk_Shape_Private;
struct _Etk_Shape
{
	Ekeko_Renderable parent;
	Etk_Shape_Private *private;
	void (*render)(Etk_Shape *s, Etk_Engine *e, Etk_Surface *surface, Etk_Context *context);
};

EAPI Ekeko_Type *etk_shape_type_get(void);
EAPI void etk_shape_color_set(Etk_Shape *s, Etk_Color color);
EAPI void etk_shape_rop_set(Etk_Shape *s, Enesim_Rop rop);
EAPI void etk_shape_filter_set(Etk_Shape *s, Etk_Filter *filter);

#define etk_shape_show(s) ekeko_renderable_show(EKEKO_RENDERABLE((s)))
#define etk_shape_hide(s) ekeko_renderable_hide(EKEKO_RENDERABLE((s)))

#endif /* ETK2_SHAPE_H_ */
