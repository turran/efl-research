/*
 * etk2_canvas.h
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */

#ifndef ETK2_CANVAS_H_
#define ETK2_CANVAS_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_CANVAS_X_CHANGED "xChanged"
#define ETK_CANVAS_Y_CHANGED "yChanged"
#define ETK_CANVAS_W_CHANGED "wChanged"
#define ETK_CANVAS_H_CHANGED "hChanged"
#define ETK_CANVAS_MATRIX_CHANGED "matrixChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_CANVAS_X;
extern Property_Id ETK_CANVAS_Y;
extern Property_Id ETK_CANVAS_W;
extern Property_Id ETK_CANVAS_H;
extern Property_Id ETK_CANVAS_MATRIX;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Canvas_Private Etk_Canvas_Private;
struct _Etk_Canvas
{
	Ekeko_Canvas parent;
	Etk_Canvas_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_canvas_type_get(void);
EAPI Etk_Canvas * etk_canvas_new(Etk_Canvas *c);
EAPI void etk_canvas_x_get(Etk_Canvas *c, Etk_Coord *x);
EAPI void etk_canvas_x_rel_set(Etk_Canvas *c, int x);
EAPI void etk_canvas_x_set(Etk_Canvas *c, int x);
EAPI void etk_canvas_y_get(Etk_Canvas *c, Etk_Coord *x);
EAPI void etk_canvas_y_set(Etk_Canvas *c, int y);
EAPI void etk_canvas_y_rel_set(Etk_Canvas *c, int y);
EAPI void etk_canvas_w_get(Etk_Canvas *c, Etk_Coord *w);
EAPI void etk_canvas_w_set(Etk_Canvas *c, int w);
EAPI void etk_canvas_w_rel_set(Etk_Canvas *c, int w);
EAPI void etk_canvas_h_get(Etk_Canvas *c, Etk_Coord *w);
EAPI void etk_canvas_h_set(Etk_Canvas *c, int h);
EAPI void etk_canvas_h_rel_set(Etk_Canvas *c, int h);
EAPI void etk_canvas_matrix_set(Etk_Canvas *c, Enesim_Matrix *m);

#endif /* ETK2_CANVAS_H_ */
