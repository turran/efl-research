#include "ESVG.h"
#include "esvg_private.h"

static void polygon_parse(ESVG *esvg)
{
	printf("polygon parse\n");
}

ESVG_Element polygon_element = {
	.tag = "polygon",
	.parser = &polygon_parse,
};
