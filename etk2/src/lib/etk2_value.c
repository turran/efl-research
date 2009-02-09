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
/* val = pointer to private->coord
 * v = Value without pointer set
 */
void _coord_set(Value *v, void *val)
{
	Etk_Coord *cv, *cval;

	cval = val;
	//cv = v->value.pointer_value;
	cv = v->value.pointer_value = malloc(sizeof(Etk_Coord));
	printf("coord set %p %p %d\n", v->value.pointer_value, val, cval->value);
	*cv = *cval;
	/* FIXME free this!!!! */
}
/* ptr = memory address to private->coord
 * v = value with pointre set
 */
void _coord_get(Value *v, void *val)
{
	Etk_Coord *cv, *cval;

	cval = val;
	cv = v->value.pointer_value;
	printf("coord pointer set %p %d => %d\n", v->value.pointer_value, cval->value, cv->value);
	*cval = *cv;
}
Eina_Bool _coord_cmp(void *a, void *b)
{
	Etk_Coord *ca = a;
	Etk_Coord *cb = b;

	if ((ca->value == cb->value) && (ca->type == cb->type))
		return EINA_FALSE;
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void etk_value_init(void)
{
	/* register the etk values */
	ETK_PROPERTY_COORD = ekeko_value_register("Etk_Coord", _coord_set,
			_coord_get, _coord_cmp);
}
void etk_value_shutdown(void)
{
	/* TODO */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int ETK_PROPERTY_COORD;
