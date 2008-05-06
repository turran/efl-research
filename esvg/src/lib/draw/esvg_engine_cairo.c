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
	cairo_t *c = context;
	
	cairo_reset_clip(c);
	//printf("process rectangle at %d %d %d %d!!\n", rclip->x, rclip->y, rclip->w, rclip->h);
	cairo_rectangle(c, rclip->x, rclip->y, rclip->w, rclip->h);
	cairo_clip(c);
	cairo_new_path(c);
	/* rounded rect */
#if 0
	if ((r->rx != 0) && (r->ry != 0))
	{
		cairo_move_to(c, r->x + r->rx, r->y);
		cairo_line_to(c, r->x + r->width - r->rx, r->y);
		cairo_curve_to(c, r->x + r->width - (r->rx/2), r->y, r->x + r->width, r->y + (r->ry/2), r->x + r->width, r->y + r->ry);
		cairo_line_to(c, r->x + r->width, r->y + r->height - r->ry);
		cairo_curve_to(c, r->x + r->width, r->y + r->height - (r->ry/2), r->x + r->width - (r->rx/2), r->y + r->height, r->x + r->width - r->rx, r->y + r->height);
		cairo_line_to(c, r->x + r->rx, r->y + r->height);
		cairo_curve_to(c, r->x + (r->rx/2), r->y + r->height, r->x, r->y + r->height - (r->ry/2), r->x, r->y + r->height - r->ry);
		cairo_line_to(c, r->x, r->y + r->ry);
		cairo_curve_to(c, r->x, r->y + (r->ry/2), r->x + (r->rx/2), r->y, r->x + r->rx, r->y);
	}
	/* regular rect */
	else
	{
		cairo_rectangle(c, r->x, r->y, r->width, r->height);
	}
#endif
	_path_fill(c, &r->shape);
	_path_stroke(c, &r->shape);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Engine esvg_engine_cairo = {
	.context_new = _context_new,
	.context_free = _context_free,
	.rect_draw = _rect_draw,
};
