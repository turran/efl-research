#include "ESVG.h"
#include "esvg_private.h"

typedef struct _ESVG_Polygon
{
	ESVG_Shape shape;
} ESVG_Polygon;


ESVG_Shape * polygon_new(void)
{
	
}

static void polygon_parse(ESVG *esvg)
{
	printf("polygon parse\n");
}

ESVG_Element polygon_element = {
	.tag = "polygon",
	.parser = &polygon_parse,
};
