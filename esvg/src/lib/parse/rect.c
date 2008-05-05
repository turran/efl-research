#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eina_Bool rect_parse(ESVG_Document *esvg)
{
	printf("rect parse\n");
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element rect_element = {
		.tag = "rect",
		.parser = &rect_parse, 
};
