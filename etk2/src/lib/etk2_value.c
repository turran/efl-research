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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void value_set(Value *v, Value_Type vtype, void *val)
{
	switch (vtype)
	{
		case PROPERTY_STRING:
		v->value.string_value = *(char **)val;
		break;

		case PROPERTY_INT:
		v->value.int_value = *(int *)val;
		break;

		case PROPERTY_RECTANGLE:
		v->value.rect = *(Eina_Rectangle *)val;
		break;

		case PROPERTY_BOOL:
		v->value.bool_value = *(Eina_Bool *)val;
		break;

		default:
		printf("VALUE SET UNDEFINED VALUE\n");
		break;
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

