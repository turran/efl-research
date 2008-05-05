#include "esvg_common.h" 
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eina_Bool group_parse(ESVG_Document *svg)
{
	printf("group parse\n");
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element group_element = {
	.tag = "group",
	.parser = &group_parse,
};
#endif