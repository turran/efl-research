/*
 * etk2_rect.h
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */

#ifndef ETK2_RECT_H_
#define ETK2_RECT_H_

typedef struct _Etk_Rect_Private Etk_Rect_Private;
struct _Etk_Rect
{
	Etk_Shape parent;
	Etk_Rect_Private *private;
};

EAPI Ekeko_Type *etk_rect_type_get(void);
EAPI void etk_rect_x_rel_set(Etk_Rect *r, int x);
EAPI void etk_rect_x_set(Etk_Rect *r, int x);
EAPI void etk_rect_y_set(Etk_Rect *r, int y);
EAPI void etk_rect_y_rel_set(Etk_Rect *r, int y);
EAPI void etk_rect_w_set(Etk_Rect *r, int w);
EAPI void etk_rect_w_rel_set(Etk_Rect *r, int w);
EAPI void etk_rect_h_set(Etk_Rect *r, int h);
EAPI void etk_rect_h_rel_set(Etk_Rect *r, int h);
#endif /* ETK2_RECT_H_ */
