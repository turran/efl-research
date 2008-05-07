#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/* TODO Rename this file to evg_core.c */
#if 0
/*
 * "1pt" equals "1.25px" (and therefore 1.25 user units)
 * "1pc" equals "15px" (and therefore 15 user units)
 * "1mm" would be "3.543307px" (3.543307 user units)
 * "1cm" equals "35.43307px" (and therefore 35.43307 user units)
 * "1in" equals "90px" (and therefore 90 user units)
 */
double units_get(const char *str)
{
	
}

typedef enum _ESVG_Unit_Type
{
	ESVG_UNIT_ABSOLUTE,
	ESVG_UNIT_RELATIVE,
	ESVG_UNITS,
} ESVG_Unit_Type;

void coord_get(const char *str, ESVG_Coord *value, ESVG_Coord_Type *type)
{
	
}

DATA32 color_get(const char *str)
{
	DATA32 color;

	sscanf(str + 1, "%x", &color);
	color |= 0xff000000;
	return color;
}
#endif
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_length_calculate(ESVG_Length *length, ESVG_Length_Value rel)
{
	/* TODO finish this function */
	switch (length->type)
	{
		case ESVG_LENGTH_TYPE_NUMBER:
			break;
		case ESVG_LENGTH_TYPE_PERCENTAGE:
			length->n_value = length->value * rel;
			break;
		case ESVG_LENGTH_TYPE_EMS:
		case ESVG_LENGTH_TYPE_EXS:
		case ESVG_LENGTH_TYPE_PX:
		case ESVG_LENGTH_TYPE_CM:
		case ESVG_LENGTH_TYPE_MM:
		case ESVG_LENGTH_TYPE_IN:
		case ESVG_LENGTH_TYPE_PT:
		case ESVG_LENGTH_TYPE_PC:
		break;
	}
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Eina_Bool esvg_length_get(const char *str, ESVG_Length *length)
{
	char units[3];
	float c;
	float r;

	//assert(length);
	
	if (!str) return EINA_FALSE;
	
	sscanf(str, "%f%s", &c, units);
	if (!strcmp(units, "cm"))
	{
		length->value = c;
		length->n_value = 35.43307 * c;
		length->type = ESVG_LENGTH_TYPE_CM;
		return EINA_TRUE;
	}
	else if (!strcmp(units, "pt"))
	{
		length->value = c;
		length->n_value = 1.25 * c;
		length->type = ESVG_LENGTH_TYPE_PT;
		return EINA_TRUE;
	}
	else if (!strcmp(units, "%"))
	{
		length->value = c;
		length->n_value = c;
		length->type = ESVG_LENGTH_TYPE_PERCENTAGE;
		return EINA_TRUE;
	}
	/* TODO do the rest */
	return EINA_FALSE;
}
