/*
 * eon_animation.h
 *
 *  Created on: 09-feb-2009
 *      Author: jl
 */

#ifndef EON_ANIMATION_H_
#define EON_ANIMATION_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_ANIMATION_START "start"
#define EON_ANIMATION_STOP "stop"
#define EON_ANIMATION_END_CHANGED "endChanged"
#define EON_ANIMATION_BEGIN_CHANGED "beginChanged"
#define EON_ANIMATION_PROPERTY_CHANGED "nameChanged"
#define EON_ANIMATION_DURATION_CHANGED "durChanged"
#define EON_ANIMATION_REPEAT_CHANGED "repeatChanged"
#define EON_ANIMATION_FROM_CHANGED "fromChanged"
#define EON_ANIMATION_TO_CHANGED "toChanged"
#define EON_ANIMATION_CALC_CHANGED "calcChanged"

typedef enum _Eon_Event_Animation_State
{
	EON_EVENT_ANIMATION_START,
	EON_EVENT_ANIMATION_STOP,
} Eon_Event_Animation_State;

typedef struct _Eon_Event_Animation
{
        Ekeko_Event event;
	Eon_Event_Animation_State state;	
} Eon_Event_Animation;
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_ANIMATION_END;
extern Ekeko_Property_Id EON_ANIMATION_BEGIN;
extern Ekeko_Property_Id EON_ANIMATION_PROPERTY;
extern Ekeko_Property_Id EON_ANIMATION_DURATION;
extern Ekeko_Property_Id EON_ANIMATION_REPEAT;
extern Ekeko_Property_Id EON_ANIMATION_FROM;
extern Ekeko_Property_Id EON_ANIMATION_TO;
extern Ekeko_Property_Id EON_ANIMATION_CALC;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef void (*Eon_Animation_Callback)(Eon_Animation *a, const char *prop,
		Ekeko_Object *o, const Etch_Data *curr, const Etch_Data *prev);

typedef struct _Eon_Animation_Private Eon_Animation_Private;
struct _Eon_Animation
{
	Ekeko_Object parent;
	Eon_Animation_Private *private;
	void (*value_set)(Ekeko_Value *v, Etch_Animation_Keyframe *k);
	void (*callback_set)(Ekeko_Value_Type vtype, Etch_Data_Type *dtype, Eon_Animation_Callback *cb);

};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_animation_type_get(void);
EAPI void eon_animation_end_set(Eon_Animation *a, Ekeko_Object *o, const char *event);
EAPI void eon_animation_begin_set(Eon_Animation *a, Ekeko_Object *o, char *event);
EAPI void eon_animation_property_set(Eon_Animation *a, const char *prop);
EAPI void eon_animation_duration_set(Eon_Animation *a, Eon_Clock *dur);
EAPI void eon_animation_repeat_set(Eon_Animation *a, int repeat);
EAPI void eon_animation_calc_set(Eon_Animation *a, Eon_Calc calc);
EAPI void eon_animation_from_set(Eon_Animation *a, Ekeko_Value *from);
EAPI void eon_animation_to_set(Eon_Animation *a, Ekeko_Value *to);


#endif /* EON_ANIMATION_H_ */
