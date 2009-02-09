/*
 * etk2_canvas.h
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */

#ifndef ETK2_CANVAS_H_
#define ETK2_CANVAS_H_

typedef struct _Etk_Canvas_Private Etk_Canvas_Private;
struct _Etk_Canvas
{
	Ekeko_Canvas parent;
	Etk_Canvas_Private *private;
};

Ekeko_Type *etk_canvas_type_get(void);
EAPI void etk_canvas_x_get(Etk_Canvas *c, Etk_Coord *x);
EAPI void etk_canvas_x_rel_set(Etk_Canvas *c, int x);
EAPI void etk_canvas_x_set(Etk_Canvas *c, int x);
EAPI void etk_canvas_y_set(Etk_Canvas *c, int y);
EAPI void etk_canvas_y_rel_set(Etk_Canvas *c, int y);
EAPI void etk_canvas_w_get(Etk_Canvas *c, Etk_Coord *w);
EAPI void etk_canvas_w_set(Etk_Canvas *c, int w);
EAPI void etk_canvas_w_rel_set(Etk_Canvas *c, int w);
EAPI void etk_canvas_h_set(Etk_Canvas *c, int h);
EAPI void etk_canvas_h_rel_set(Etk_Canvas *c, int h);

#endif /* ETK2_CANVAS_H_ */
