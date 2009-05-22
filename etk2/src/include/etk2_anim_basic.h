/*
 * etk2_anim_basic.h
 *
 *  Created on: 22-may-2009
 *      Author: jl
 */

#ifndef ETK2_ANIM_BASIC_H_
#define ETK2_ANIM_BASIC_H_
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Animation_Basic_Private Etk_Animation_Basic_Private;
struct _Etk_Animation_Basic
{
	Etk_Animation parent;
	Etk_Animation_Basic_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type * etk_animation_basic_type_get(void);
EAPI Etk_Animation_Basic * etk_animation_basic_new(void);

#endif /* ETK2_ANIM_BASIC_H_ */
