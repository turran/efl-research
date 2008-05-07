#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
static Eina_Bool polygon_parse(ESVG_Document *esvg)
{
	printf("polygon parse\n");
}

ESVG_Document_Element polygon_element = {
		.element = ESVG_ELEMENT_POLYGON,
		.parser = &polygon_parse, 
};
