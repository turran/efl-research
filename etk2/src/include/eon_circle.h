/*
 * eon_circle.h
 *
 *  Created on: 28-jun-2009
 *      Author: jl
 */
#ifndef EON_CIRCLE_H_
#define EON_CIRCLE_H_

/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Circle_Private Eon_Circle_Private;
struct _Eon_Circle
{
	Eon_Shape parent;
	Eon_Circle_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_circle_type_get(void);
EAPI Eon_Circle * eon_circle_new(Eon_Canvas *c);
EAPI void eon_circle_x_rel_set(Eon_Square *r, int x);
EAPI void eon_circle_x_set(Eon_Square *r, int x);
EAPI void eon_circle_y_set(Eon_Square *r, int y);
EAPI void eon_circle_y_rel_set(Eon_Square *r, int y);
EAPI void eon_circle_radius_set(Eon_Square *r, int r);
EAPI void eon_circle_radius_rel_set(Eon_Square *r, int r);

#endif /* EON_CIRCLE_H_ */
