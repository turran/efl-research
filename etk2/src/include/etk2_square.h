/*
 * etk2_square.h
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */

#ifndef ETK2_SQUARE_H_
#define ETK2_SQUARE_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_SQUARE_X_CHANGED "xChanged"
#define ETK_SQUARE_Y_CHANGED "yChanged"
#define ETK_SQUARE_W_CHANGED "wChanged"
#define ETK_SQUARE_H_CHANGED "hChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_SQUARE_X;
extern Property_Id ETK_SQUARE_Y;
extern Property_Id ETK_SQUARE_W;
extern Property_Id ETK_SQUARE_H;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Square_Private Etk_Square_Private;
struct _Etk_Square
{
	Etk_Shape parent;
	Etk_Square_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_square_type_get(void);
EAPI void etk_square_x_rel_set(Etk_Square *r, int x);
EAPI void etk_square_x_set(Etk_Square *r, int x);
EAPI void etk_square_y_set(Etk_Square *r, int y);
EAPI void etk_square_y_rel_set(Etk_Square *r, int y);
EAPI void etk_square_w_set(Etk_Square *r, int w);
EAPI void etk_square_w_rel_set(Etk_Square *r, int w);
EAPI void etk_square_h_set(Etk_Square *r, int h);
EAPI void etk_square_h_rel_set(Etk_Square *r, int h);
EAPI void etk_square_coords_get(Etk_Square *s, Etk_Coord *x, Etk_Coord *y, Etk_Coord *w, Etk_Coord *h);


#endif /* ETK2_SQUARE_H_ */
