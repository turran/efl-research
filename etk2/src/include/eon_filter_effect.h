/*
 * eon_filter_effect.h
 *
 *  Created on: 07-may-2009
 *      Author: jl
 */

#ifndef EON_FILTER_EFFECT_H_
#define EON_FILTER_EFFECT_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/

/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_FILTER_EFFECT_X;
extern Ekeko_Property_Id EON_FILTER_EFFECT_Y;
extern Ekeko_Property_Id EON_FILTER_EFFECT_W;
extern Ekeko_Property_Id EON_FILTER_EFFECT_H;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Filter_Effect_Private Eon_Filter_Effect_Private;
struct _Eon_Filter_Effect
{
	Eon_Square parent;
	Eon_Filter_Effect_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_filter_effect_type_get(void);

#endif /* EON_FILTER_EFFECT_H_ */
