#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void * _context_new(void *engine_data)
{
	cairo_t *c;
	cairo_surface_t *cs = engine_data;
	
	c = cairo_create(cs);
	
	return c;
}

static void _context_free(void *engine_data, void *context)
{
	cairo_t *c = context;
	cairo_destroy(c);
}

static void _color_to_double(ESVG_Color color, double *r, double *g, double *b)
{
	if (r) *r = ((color & 0x00ff0000) >> 16) / 255.0;
	if (g) *g = ((color & 0x0000ff00) >> 8) / 255.0;
	if (b) *b = (color & 0x000000ff) / 255.0;
}

static void  _path_stroke(cairo_t *c, ESVG_Shape *s)
{
	double opacity = s->attributes.opacity.stroke_opacity;
	double r, g, b;
		
	if (opacity)
	{
		_color_to_double(s->attributes.paint.stroke, &r, &g, &b);
		cairo_set_source_rgba(c, r, g, b, opacity);
		cairo_set_line_width(c, 4.0);
		cairo_stroke(c);
	}
}

static void  _path_fill(cairo_t *c, ESVG_Shape *s)
{
	double opacity = s->attributes.opacity.fill_opacity;
	double r, g, b;
	
	if (opacity) 
	{
		_color_to_double(s->attributes.paint.fill, &r, &g, &b);
		cairo_set_source_rgba(c, r, g, b, opacity);
		
		if (s->attributes.opacity.stroke_opacity)
		{
			cairo_fill_preserve(c);
		}
		else
		{
			cairo_fill(c);
		}
	}
}

static void _rect_draw(void *engine_data, void *context, ESVG_Rect *r, Eina_Rectangle *rclip)
{
	ESVG_Length_Value x, y, w, h, rx, ry;
	cairo_t *c = context;
	
	cairo_reset_clip(c);
	//printf("process rectangle at %d %d %d %d!!\n", rclip->x, rclip->y, rclip->w, rclip->h);
	cairo_rectangle(c, rclip->x, rclip->y, rclip->w, rclip->h);
	cairo_clip(c);
	cairo_new_path(c);
	
	x = r->x.value;
	y = r->y.value;
	w = r->width.value;
	h = r->height.value;
	rx = r->rx.value;
	ry = r->ry.value;
	/* rounded rect */
	if ((rx != 0) && (ry != 0))
	{
		cairo_move_to(c, x + rx, y);
		cairo_line_to(c, x + w - rx, y);
		cairo_curve_to(c, x + w - (rx/2), y, x + w, y + (ry/2), x + w, y + ry);
		cairo_line_to(c, x + w, y + h - ry);
		cairo_curve_to(c, x + w, y + h - (ry/2), x + w - (rx/2), y + h, x + w - rx, y + h);
		cairo_line_to(c, x + rx, y + h);
		cairo_curve_to(c, x + (rx/2), y + h, x, y + h - (ry/2), x, y + h - ry);
		cairo_line_to(c, x, y + ry);
		cairo_curve_to(c, x, y + (ry/2), x + (rx/2), y, x + rx, y);
	}
	/* regular rect */
	else
	{
		cairo_rectangle(c, x, y, w, h);
	}
	printf("painting rect\n");
	//_path_fill(c, &r->shape);
	//_path_stroke(c, &r->shape);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Engine_Func esvg_engine_cairo = {
	.context_new = _context_new,
	.context_free = _context_free,
	.rect_draw = _rect_draw,
};
