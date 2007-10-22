#include "ESVG.h"
#include "esvg_private.h"

static void group_parse(ESVG *svg)
{
	printf("group parse\n");
}

ESVG_Element group_element = {
	.tag = "group",
	.parser = &group_parse,
};