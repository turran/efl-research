/*
 * etk2_animation.h
 *
 *  Created on: 09-feb-2009
 *      Author: jl
 */

#ifndef ETK2_ANIMATION_H_
#define ETK2_ANIMATION_H_

typedef struct _Etk_Animation_Private Etk_Animation_Private;
struct _Etk_Animation
{
	Ekeko_Object parent;
	Etk_Animation_Private *private;
};

EAPI Ekeko_Type *etk_animation_type_get(void);
EAPI Etk_Animation * etk_animation_new(void);
EAPI void etk_animation_end_set(Etk_Animation *a, Ekeko_Object *o, const char *event);
EAPI void etk_animation_begin_set(Etk_Animation *a, Ekeko_Object *o, char *event);
EAPI void etk_animation_property_set(Etk_Animation *a, const char *prop);
EAPI void etk_animation_duration_set(Etk_Animation *a, Etk_Clock *dur);
EAPI void etk_animation_repeat_set(Etk_Animation *a, int repeat);
EAPI void etk_animation_calc_set(Etk_Animation *a, Etk_Calc calc);
EAPI void etk_animation_from_set(Etk_Animation *a, Ekeko_Value *from);
EAPI void etk_animation_to_set(Etk_Animation *a, Ekeko_Value *to);


#endif /* ETK2_ANIMATION_H_ */
