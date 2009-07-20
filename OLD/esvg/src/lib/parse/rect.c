#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eina_Bool rect_parse(ESVG_Parser *svg, ESVG_Element *parent, ESVG_Element **new_element)
{
	char *attr;
	ESVG_Element *r;
	ESVG_Length w, h, x, y, rx, ry;
	
	/* TODO change this to add the rect to the parent object */
	r = esvg_rect_new();
	/* parse the attributes */
	attr = exml_attribute_get(svg->xml, "width");
	if (!esvg_length_get(attr, &w))
		goto error;
	attr = exml_attribute_get(svg->xml, "height");
	if (!esvg_length_get(attr, &h))
		goto error;
	attr = exml_attribute_get(svg->xml, "x");
	if (!esvg_length_get(attr, &x))
	{
		x.value = 0;
		x.type = ESVG_LENGTH_TYPE_NUMBER;
	}
	attr = exml_attribute_get(svg->xml, "y");
	if (!esvg_length_get(attr, &y))
	{
		y.value = 0;
		y.type = ESVG_LENGTH_TYPE_NUMBER;
	}
	esvg_rect_geometry_set(ESVG_RECT(r), &x, &y, &w, &h);
	/* rx and ry */
#if 0
	attr = exml_attribute_get(svg->xml, "rx");
	if (!esvg_length_get(attr, &h))
		goto error;
#endif
	/* common shape attributes */
	//esvg_shape_parse(svg, esvg_rect_shape_get(r));
	*new_element = r;
	
	return EINA_TRUE;
error:
	printf("error\n");
	esvg_rect_del(r);
	return EINA_FALSE;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element element_rect = {
		.element = ESVG_ELEMENT_RECT,
		.parser = &rect_parse,
};
