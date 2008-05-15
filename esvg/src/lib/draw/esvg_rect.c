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
	//esvg_engine_rect_draw(rect->shape.canvas, rect->shape.engine.context, rect, r);
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
/* Classs */
static void _constructor(ESVG_Svg *s)
{
	printf("rect constructor\n");
}
static void _destructor(ESVG_Svg *s)
{
	
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eobj_Class * esvg_rect_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		c = eobj_class_new("ESVG_Rect", ESVG_SHAPE_CLASS,
				sizeof(ESVG_Rect), EOBJ_CONSTRUCTOR(_constructor),
				EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;
}
/* relative properties (x, y, w, h) should register the notifiers */
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI ESVG_Rect * esvg_rect_new(void)
{
	return (ESVG_Rect *)esvg_element_new(ESVG_RECT_CLASS, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord *x, ESVG_Coord *y, ESVG_Length *w, ESVG_Length *h)
{
	if (x) r->x = *x;
	if (y) r->y = *y;
	if (w) r->width = *w;
	if (h) r->height = *h;
	/* TODO check if some of the values are relative, if so register callbacks
	 * to the parent */
	printf("new rect of size %f %f %f %f\n", x->value, y->value, w->value, h->value);
	//ekeko_canvas_callback_add(r->shape.canvas->canvas, EKEKO_CANVAS_EVENT_RESIZE, NULL); 
	//esvg_shape_geometry_set(&r->shape, x->value, y->value, w->value, h->value);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry)
{
	r->rx = rx;
	r->ry = ry;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_rect_del(ESVG_Rect *r)
{
	
}
