#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
Eina_Bool svg_parse(ESVG_Parser *parser, ESVG_Element *parent, ESVG_Element **new_element)
{
	ESVG_Element *s;
	ESVG_Length w, h;
	ESVG_Element *svg;
	char *attr;
	
	svg = esvg_svg_new();
	/* canvas */
	if (!parent)
	{
		esvg_svg_engine_set(svg, parser->engine);
		parser->svg = ESVG_SVG(svg);
	}
	/* parse the attributes */
	attr = exml_attribute_get(parser->xml, "width");
	if (!esvg_length_get(attr, &w))
	{
		w.value = 100;
		w.type = ESVG_LENGTH_TYPE_PERCENTAGE;
	}
	attr = exml_attribute_get(parser->xml, "height");
	if (!esvg_length_get(attr, &h))
	{
		h.value = 100;
		h.type = ESVG_LENGTH_TYPE_PERCENTAGE;
	}
	esvg_svg_width_set(svg, &w);
	esvg_svg_height_set(svg, &h);
	/* canvas or subcanvas ? */
	if (parent)
	{
		ESVG_Container *c;
		
		esvg_container_add(ESVG_CONTAINER(parent), svg);
	}
	*new_element = svg;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element element_svg = {
	.element = ESVG_ELEMENT_SVG,
	.parser = &svg_parse,
	.children = { ESVG_ELEMENT_SVG, ESVG_ELEMENT_G, ESVG_ELEMENT_POLYGON, ESVG_ELEMENT_RECT},
};
