#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_shape_parse(ESVG_Document *svg, ESVG_Shape *shape)
{
	char *attr;
	ESVG_Length_Value op;
	ESVG_Color col;
	
	/* opacity attributes */
	attr = exml_attribute_get(svg->xml, "opacity");
	if (esvg_opacity_get(attr, &op))
	{
		esvg_shape_opacity_set(shape, op);
	}
	/* paint attributes */
	attr = exml_attribute_get(svg->xml, "fill");
	if (esvg_color_get(attr, &col))
	{
		esvg_shape_fill_set(shape, col);
	}
}