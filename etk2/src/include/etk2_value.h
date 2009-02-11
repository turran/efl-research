/*
 * etk_value.h
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */

#ifndef ETK_VALUE_H_
#define ETK_VALUE_H_

extern Ekeko_Value_Type ETK_PROPERTY_COORD;
extern Ekeko_Value_Type ETK_PROPERTY_CLOCK;
extern Ekeko_Value_Type ETK_PROPERTY_TRIGGER;

typedef enum
{
	ETK_COORD_ABSOLUTE,
	ETK_COORD_RELATIVE,
} Etk_Coord_Type;

typedef struct _Etk_Coord
{
	int final;
	Etk_Coord_Type type;
	int value;
} Etk_Coord;


typedef struct _Etk_Trigger
{
	Ekeko_Object *obj;
	char *event;
} Etk_Trigger;

typedef struct _Etk_Clock
{
	unsigned int seconds;
	unsigned int micro; /* 10^-6 */
} Etk_Clock;

typedef enum _Etk_Calc
{
	ETK_CALC_LINEAR,
	ETK_CALC_DISCRETE,
} Etk_Calc;

/* TODO Normalize the below functions */
static inline void etk_value_coord_from(Ekeko_Value *v, Etk_Coord *coord, int value, Etk_Coord_Type type)
{
	coord->value = value;
	coord->type = type;
	v->value.pointer_value = coord;
	v->type = ETK_PROPERTY_COORD;
}

static inline void etk_value_clock_from(Ekeko_Value *v, Etk_Clock *clock)
{
	v->value.pointer_value = clock;
	v->type = ETK_PROPERTY_CLOCK;
}

static inline void etk_value_clock_seconds_from(Ekeko_Value *v, Etk_Clock *clock, unsigned int sec)
{
	clock->seconds = sec;
	clock->micro = 0;
	v->value.pointer_value = clock;
	v->type = ETK_PROPERTY_CLOCK;
}

static inline void etk_value_clock_micro_from(Ekeko_Value *v, Etk_Clock *clock, unsigned int micro)
{
	clock->seconds = 0;
	clock->micro = micro;
	v->value.pointer_value = clock;
	v->type = ETK_PROPERTY_CLOCK;
}

static inline void etk_value_trigger_from(Ekeko_Value *v, Etk_Trigger *t)
{
	v->value.pointer_value = t;
	v->type = ETK_PROPERTY_TRIGGER;
}
#endif /* ETK_VALUE_H_ */
