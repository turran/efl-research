#include "ESVG.h"
#include "esvg_private.h"

/*
 * "1pt" equals "1.25px" (and therefore 1.25 user units)
 * "1pc" equals "15px" (and therefore 15 user units)
 * "1mm" would be "3.543307px" (3.543307 user units)
 * "1cm" equals "35.43307px" (and therefore 35.43307 user units)
 * "1in" equals "90px" (and therefore 90 user units)
 */
float units_get(const char *str)
{
	char units[3];
	float c;
	float r;

	assert(str);
	sscanf(str, "%f%s", &c, units);
	if (!strcmp(units, "cm"))
	{
		r = 35.43307 * c; 	
	}
	else if (!strcmp(units, "pt"))
	{
		r = 1.25 * c;
	}
	return r;
}

DATA32 color_get(const char *str)
{
	DATA32 color;

	sscanf(str + 1, "%x", &color);
	color |= 0xff000000;
	return color;
}
