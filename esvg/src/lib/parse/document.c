#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
Eina_Bool document_parse(ESVG_Parser *svg, ESVG_Element *parent)
{
	ESVG_Element *s;
	char *attr;
	
	/* canvas or subcanvas ? */
	if (!svg->document)
	{
#if 0
		ESVG_Length w, h;
		
		svg->canvas = esvg_svg_new();
		esvg_engine_set(svg->canvas, svg->type, svg->engine_data);
		esvg_output_size_set(svg->canvas, svg->w, svg->h);
		/* parse the attributes */
		attr = exml_attribute_get(svg->xml, "width");
		if (!esvg_length_get(attr, &w))
		{
			w.value = 100;
			w.type = ESVG_LENGTH_TYPE_PERCENTAGE;
		}
		attr = exml_attribute_get(svg->xml, "height");
		if (!esvg_length_get(attr, &h))
		{
			h.value = 100;
			h.type = ESVG_LENGTH_TYPE_PERCENTAGE;
		}
		esvg_size_set(svg->canvas, &w, &h);	
		/* Lets begin the recursion */
		element_child_parse(svg, &element_document);
#endif
		s = esvg_svg_new();
	}
	else
	{
		printf("this should be a subcanvas!!!\n");
	}
	/* parse the childs */
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element element_document = {
	.element = ESVG_ELEMENT_SVG,
	.parser = &document_parse,
	.children = { ESVG_ELEMENT_SVG, ESVG_ELEMENT_G, ESVG_ELEMENT_POLYGON, ESVG_ELEMENT_RECT},
};
