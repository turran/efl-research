/*
 * etk2_filter_effect.h
 *
 *  Created on: 07-may-2009
 *      Author: jl
 */

#ifndef ETK2_FILTER_EFFECT_H_
#define ETK2_FILTER_EFFECT_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/

/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_FILTER_EFFECT_X;
extern Property_Id ETK_FILTER_EFFECT_Y;
extern Property_Id ETK_FILTER_EFFECT_W;
extern Property_Id ETK_FILTER_EFFECT_H;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Filter_Effect_Private Etk_Filter_Effect_Private;
struct _Etk_Filter_Effect
{
	Etk_Square parent;
	Etk_Filter_Effect_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_filter_effect_type_get(void);

#endif /* ETK2_FILTER_EFFECT_H_ */
