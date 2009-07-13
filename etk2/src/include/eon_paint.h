/*
 * eon_paint.h
 *
 *  Created on: 13-jul-2009
 *      Author: jl
 */

#ifndef EON_PAINT_H_
#define EON_PAINT_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_PAINT_X_CHANGED "xChanged"
#define EON_PAINT_Y_CHANGED "yChanged"
#define EON_PAINT_W_CHANGED "wChanged"
#define EON_PAINT_H_CHANGED "hChanged"
#define EON_PAINT_MATRIX_CHANGED "matrixChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_PAINT_X;
extern Ekeko_Property_Id EON_PAINT_Y;
extern Ekeko_Property_Id EON_PAINT_W;
extern Ekeko_Property_Id EON_PAINT_H;
extern Ekeko_Property_Id EON_PAINT_MATRIX;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Paint_Private Eon_Paint_Private;
struct _Eon_Paint
{
	Ekeko_Object parent;
	Eon_Paint_Private *private;
	/* Called whenever a shape object references a paint object */
	void *(*create)(Eon_Engine *e, Eon_Paint *p);
	/* Called whenever a shape needs to be rendered and has a paint object as
	 * the fill property */
	void (*setup)(Eon_Engine *e, void *engine_data, Eon_Shape *s);
	/* Called whenever the last shape object unreferences this paint object */
	void (*delete)(Eon_Engine *e, void *engine_data);
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_paint_type_get(void);
EAPI void eon_paint_coords_get(Eon_Paint *p, Eon_Coord *x, Eon_Coord *y, Eon_Coord *w, Eon_Coord *h);

#endif /* EON_PAINT_H_ */
