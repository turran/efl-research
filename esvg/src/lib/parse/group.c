#include "ESVG.h"
#include "esvg_private.h"

typedef struct _ESVG_Element
{
	ESVG_element element;
} ESVG_Element;

#if 0
static void group_parse(ESVG *svg)
{
	printf("group parse\n");
}

ESVG_Element group_element = {
	.tag = "group",
	.parser = &group_parse,
};
#endif