/*
 * etk2_value.c
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/*                                  Coord                                     *
 *============================================================================*/
/* val = pointer to private->coord
 * v = Ekeko_Value without pointer set
 */
void * _coord_create(void)
{
	return malloc(sizeof(Etk_Coord));
}
void _coord_pointer_from(Ekeko_Value *v, Etk_Coord *val)
{
	Etk_Coord *cv = v->value.pointer_value;

	*cv = *val;
}
/* ptr = memory address to private->coord
 * v = value with pointre set
 */
void _coord_pointer_to(Ekeko_Value *v, Etk_Coord *val)
{
	Etk_Coord *cv = v->value.pointer_value;

	*val = *cv;
}
Eina_Bool _coord_cmp(Etk_Coord *a, Etk_Coord *b)
{
	if ((a->value == b->value) && (a->type == b->type))
		return EINA_FALSE;
	return EINA_TRUE;
}

void _coord_free(Etk_Coord *v)
{
	free(v);
}
/*                                  Clock                                     *
 *============================================================================*/
void * _clock_create(void)
{
	return malloc(sizeof(Etk_Clock));
}

void _clock_pointer_from(Ekeko_Value *v, Etk_Clock *val)
{
	Etk_Clock *cv = v->value.pointer_value;

	*cv = *val;
}

void _clock_pointer_to(Ekeko_Value *v, Etk_Clock *val)
{
	Etk_Clock *cv = v->value.pointer_value;
	*val = *cv;
}

Eina_Bool _clock_cmp(Etk_Clock *a, Etk_Clock *b)
{
	if ((a->seconds == b->seconds) && (a->micro == b->micro))
		return EINA_FALSE;
	return EINA_TRUE;
}

void _clock_free(Etk_Clock *v)
{
	free(v);
}

/*                                 Trigger                                    *
 *============================================================================*/
void * _trigger_create(void)
{
	return malloc(sizeof(Etk_Trigger));
}

void _trigger_pointer_from(Ekeko_Value *v, Etk_Trigger *t)
{
	Etk_Trigger *cv = v->value.pointer_value;

	*cv = *t;
}

void _trigger_pointer_to(Ekeko_Value *v, Etk_Trigger *t)
{
	Etk_Trigger *cv = v->value.pointer_value;
	*t = *cv;
#if 0
	Etk_Trigger *cv, *cval;

	cval = val;
	cv = v->value.pointer_value = malloc(sizeof(Etk_Trigger));
	if (cval->event)
		cv->event = strdup(cval->event);
	cv->obj = cval->obj;
#endif
}


Eina_Bool _trigger_cmp(Etk_Trigger *a, Etk_Trigger *b)
{
	if ((a->obj == b->obj) && (!strcmp(a->event, b->event)))
		return EINA_FALSE;
	return EINA_TRUE;
}

void _trigger_free(Etk_Trigger *t)
{
	free(t);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void etk_value_init(void)
{
	/* register the etk values */
	ETK_PROPERTY_COORD = ekeko_value_register("Etk_Coord",
			EKEKO_VALUE_CREATE(_coord_create),
			EKEKO_VALUE_FREE(_coord_free),
			EKEKO_VALUE_CMP(_coord_cmp),
			EKEKO_VALUE_POINTER_FROM(_coord_pointer_from),
			EKEKO_VALUE_POINTER_TO(_coord_pointer_to));
	ETK_PROPERTY_CLOCK = ekeko_value_register("Etk_Clock",
			EKEKO_VALUE_CREATE(_clock_create),
			EKEKO_VALUE_FREE(_clock_free),
			EKEKO_VALUE_CMP(_clock_cmp),
			EKEKO_VALUE_POINTER_FROM(_clock_pointer_from),
			EKEKO_VALUE_POINTER_TO(_clock_pointer_to));
	ETK_PROPERTY_TRIGGER = ekeko_value_register("Etk_Trigger",
			EKEKO_VALUE_CREATE(_trigger_create),
			EKEKO_VALUE_FREE(_trigger_free),
			EKEKO_VALUE_CMP(_trigger_cmp),
			EKEKO_VALUE_POINTER_FROM(_trigger_pointer_from),
			EKEKO_VALUE_POINTER_TO(_trigger_pointer_to));
}
void etk_value_shutdown(void)
{
	/* TODO */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Value_Type ETK_PROPERTY_COORD;
Ekeko_Value_Type ETK_PROPERTY_CLOCK;
Ekeko_Value_Type ETK_PROPERTY_TRIGGER;
