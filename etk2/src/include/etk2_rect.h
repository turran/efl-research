/*
 * etk2_rect.h
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */

#ifndef ETK2_RECT_H_
#define ETK2_RECT_H_
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Rect_Private Etk_Rect_Private;
struct _Etk_Rect
{
	Etk_Square parent;
	Etk_Rect_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_rect_type_get(void);
EAPI Etk_Rect * etk_rect_new(Etk_Canvas *c);

/* square wrappers */
#define etk_rect_x_rel_set(r, x) etk_square_x_rel_set((Etk_Square *)(r), x)
#define etk_rect_y_rel_set(r, y) etk_square_y_rel_set((Etk_Square *)(r), y)
#define etk_rect_w_rel_set(r, w) etk_square_w_rel_set((Etk_Square *)(r), w)
#define etk_rect_h_rel_set(r, h) etk_square_h_rel_set((Etk_Square *)(r), h)
#define etk_rect_x_set(r, x) etk_square_x_set((Etk_Square *)(r), x)
#define etk_rect_y_set(r, y) etk_square_y_set((Etk_Square *)(r), y)
#define etk_rect_w_set(r, w) etk_square_w_set((Etk_Square *)(r), w)
#define etk_rect_h_set(r, h) etk_square_h_set((Etk_Square *)(r), h)
/* shape wrappers */
#define etk_rect_color_set(r, c) etk_shape_color_set((Etk_Shape *)(r), c)
#define etk_rect_rop_set(r, o) etk_shape_rop_set((Etk_Shape *)(r), o)
/* renderable wrappers */
#define etk_rect_show(r) ekeko_renderable_show(EKEKO_RENDERABLE((r)))
#define etk_rect_hide(r) ekeko_renderable_hide(EKEKO_RENDERABLE((r)))

#endif /* ETK2_RECT_H_ */
