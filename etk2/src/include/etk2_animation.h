/*
 * etk2_animation.h
 *
 *  Created on: 09-feb-2009
 *      Author: jl
 */

#ifndef ETK2_ANIMATION_H_
#define ETK2_ANIMATION_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_ANIMATION_END_CHANGED "endChanged"
#define ETK_ANIMATION_BEGIN_CHANGED "beginChanged"
#define ETK_ANIMATION_PROPERTY_CHANGED "nameChanged"
#define ETK_ANIMATION_DURATION_CHANGED "durChanged"
#define ETK_ANIMATION_REPEAT_CHANGED "repeatChanged"
#define ETK_ANIMATION_FROM_CHANGED "fromChanged"
#define ETK_ANIMATION_TO_CHANGED "toChanged"
#define ETK_ANIMATION_CALC_CHANGED "calcChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_ANIMATION_END;
extern Property_Id ETK_ANIMATION_BEGIN;
extern Property_Id ETK_ANIMATION_PROPERTY;
extern Property_Id ETK_ANIMATION_DURATION;
extern Property_Id ETK_ANIMATION_REPEAT;
extern Property_Id ETK_ANIMATION_FROM;
extern Property_Id ETK_ANIMATION_TO;
extern Property_Id ETK_ANIMATION_CALC;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef void (*Etk_Animation_Callback)(Etk_Animation *a, const char *prop,
		Ekeko_Object *o, const Etch_Data *curr, const Etch_Data *prev);

typedef struct _Etk_Animation_Private Etk_Animation_Private;
struct _Etk_Animation
{
	Ekeko_Object parent;
	Etk_Animation_Private *private;
	void (*value_set)(Ekeko_Value *v, Etch_Animation_Keyframe *k);
	void (*callback_set)(Ekeko_Value_Type vtype, Etch_Data_Type *dtype, Etk_Animation_Callback *cb);

};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_animation_type_get(void);
EAPI void etk_animation_end_set(Etk_Animation *a, Ekeko_Object *o, const char *event);
EAPI void etk_animation_begin_set(Etk_Animation *a, Ekeko_Object *o, char *event);
EAPI void etk_animation_property_set(Etk_Animation *a, const char *prop);
EAPI void etk_animation_duration_set(Etk_Animation *a, Etk_Clock *dur);
EAPI void etk_animation_repeat_set(Etk_Animation *a, int repeat);
EAPI void etk_animation_calc_set(Etk_Animation *a, Etk_Calc calc);
EAPI void etk_animation_from_set(Etk_Animation *a, Ekeko_Value *from);
EAPI void etk_animation_to_set(Etk_Animation *a, Ekeko_Value *to);


#endif /* ETK2_ANIMATION_H_ */
