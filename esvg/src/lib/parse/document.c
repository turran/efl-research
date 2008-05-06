#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
Eina_Bool document_parse(ESVG_Document *svg)
{
	ESVG *esvg;
	char *attr;
	
	/* canvas or subcanvas ? */
	if (!svg->canvas)
	{
		/* parse the attributes */
		attr = exml_attribute_get(svg->xml, "width");
		
		if (!attr)
			printf("no attribute\n");
		else
			printf("value = %s\n", attr);
		attr = exml_attribute_get(svg->xml, "height");
		if (!attr)
			printf("no attribute\n");
		else
			printf("value = %s\n", attr);
		//svg->canvas = esvg_new(w, h, svg->type, svg->data);
	}
	printf("ok??\n");
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element document_element = {
	.tag = "svg",
	.parser = &document_parse,
	.children = { ESVG_ELEMENT_DOCUMENT, ESVG_ELEMENT_GROUP, ESVG_ELEMENT_POLYGON},
};
