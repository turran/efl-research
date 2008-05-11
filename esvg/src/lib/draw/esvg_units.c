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
		
		/* relative uints */
		case ESVG_LENGTH_TYPE_PERCENTAGE:
			length->value = length->type_value * rel;
			break;
		/* absolute units */
		case ESVG_LENGTH_TYPE_NUMBER:
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
EAPI Eina_Bool esvg_opacity_get(const char *str, ESVG_Length_Value *op)
{
	float f;
	
	if (!str) return EINA_FALSE;
	
	sscanf(str, "%f", &f);
	if (f < 0.0)
		*op = 0.0;
	else if (f > 1.0)
		*op = 1.0;
	else
		*op = f;
	return EINA_TRUE;
}

EAPI Eina_Bool esvg_color_get(const char *str, ESVG_Color *color)
{
	if (!str) return EINA_FALSE;
	
	/* rgb(r, g, b)*/
	/* three digits hex 0xfb0 = 0xffbb00 */
	/* six digits hex 0xffbb00 */
	/* rgb(100%, 50%, 50%) */
	/* color definitions, cyan, red, black, etc */
	if (!strcmp(str, "blue"))
		*color = 0x0000ff;	
	else if (!strcmp(str, "red"))
		*color = 0xff0000;
	else
		return EINA_FALSE;
	return EINA_TRUE;
}
EAPI Eina_Bool esvg_length_get(const char *str, ESVG_Length *length)
{
	char *units;
	int num;
	float c;
	float r;

	//assert(length);
	
	if (!str) return EINA_FALSE;
	
	sscanf(str, "%f%n", &c, &num);
	units = (char *)str + num;
	if (!(*units))
	{
		length->type_value = c;
		length->value = c;
		length->type = ESVG_LENGTH_TYPE_NUMBER;
	}
	else if (!strcmp(units, "cm"))
	{
		length->type_value =  c;
		length->value = 35.43307 * c;
		length->type = ESVG_LENGTH_TYPE_CM;
	}
	else if (!strcmp(units, "pt"))
	{
		length->type_value = c;
		length->value = 1.25 * c;
		length->type = ESVG_LENGTH_TYPE_PT;
	}
	else if (!strcmp(units, "%"))
	{
		length->type_value = c;
		length->value = c;
		length->type = ESVG_LENGTH_TYPE_PERCENTAGE;
	}
	else
		return EINA_FALSE;
	/* TODO do the rest */
	return EINA_TRUE;
}
