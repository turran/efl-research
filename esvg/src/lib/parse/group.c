#include "esvg_common.h" 
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eina_Bool group_parse(ESVG_Document *svg, ESVG_Element *parent)
{
	printf("group parse\n");
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element element_group = {
	.element = ESVG_ELEMENT_G,
	.parser = &group_parse,
	.children = { ESVG_ELEMENT_PATH, ESVG_ELEMENT_TEXT, ESVG_ELEMENT_RECT,
		ESVG_ELEMENT_CIRCLE, ESVG_ELEMENT_ELLIPSE, ESVG_ELEMENT_LINE,
		ESVG_ELEMENT_POLYLINE, ESVG_ELEMENT_POLYGON, ESVG_ELEMENT_IMAGE,
		ESVG_ELEMENT_USE },
};
