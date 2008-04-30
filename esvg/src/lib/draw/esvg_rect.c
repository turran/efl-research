#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/* rect
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, x, y, width, height, rx, ry, transform 
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _free(void *data)
{
	printf("free\n");
}

static void _pre_process(void *data)
{
	printf("pre process!!\n");
}

static void _process(void *data, Eina_Rectangle *r)
{
	ESVG_Rect *rect = data;
	
	printf("process %d %d %d %d!!\n", r->x, r->y, r->w, r->h);
	esvg_engine_rect_draw(rect->shape.canvas, rect->shape.engine.context, rect, r);
}

static void _post_process(void *data)
{
	printf("post process!!\n");
}

Ekeko_Object_Class _rect_class = {
	.name = "svg:rect",
	.pre_process = _pre_process,
	.process = _process,
	.post_process = _post_process,
	.free = _free,
};
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG_Rect * esvg_rect_add(ESVG *svg)
{
	ESVG_Rect *r;
	
	r = calloc(1, sizeof(ESVG_Rect));
	esvg_canvas_shape_add(svg, &r->shape, &_rect_class, r);
	
	return r;
}

EAPI ESVG_Shape * esvg_rect_shape_get(ESVG_Rect *r)
{
	return &r->shape;
}

EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord x, ESVG_Coord y, ESVG_Length width, ESVG_Length height)
{
	r->x = x;
	r->y = y;
	r->width = width;
	r->height = height;
	esvg_shape_geometry_set(&r->shape, x, y, width, height);
}

EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry)
{
	r->rx = rx;
	r->ry = ry;
}
