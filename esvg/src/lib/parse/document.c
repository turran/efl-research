#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
Eina_Bool document_parse(ESVG_Document *svg)
{
	ESVG *esvg;
	ESVG_Length length;
	
	char *attr;
	
	/* canvas or subcanvas ? */
	if (!svg->canvas)
	{
		/* parse the attributes */
		attr = exml_attribute_get(svg->xml, "width");
		esvg_length_get(attr, &length);
		printf("%f %d\n", length.value, length.type);
		
		attr = exml_attribute_get(svg->xml, "height");
		esvg_length_get(attr, &length);
		printf("%f %d\n", length.value, length.type);
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
