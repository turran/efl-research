#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Fill_Color
{
	DATA32	color;
} Fill_Color;

static void _draw_alias(Enesim_Renderer *r, Enesim_Scanline_Alias *sl, Enesim_Surface *dst)
{
	Fill_Color *f;
	Enesim_Drawer_Span cfnc;
	Enesim_Surface_Data ddata;
	Enesim_Surface_Format sfmt;
	int nsl;
	int offset;
	int i;

	f = r->data;
	
	sfmt = enesim_surface_format_get(dst);
	cfnc = enesim_drawer_span_color_get(r->rop, sfmt, f->color);
	offset = (dst->w * sl->y) + sl->x;
	enesim_surface_data_get(dst, &ddata);
	enesim_surface_data_increment(&ddata, sfmt, offset);
	cfnc(&ddata, sl->w, NULL, f->color, NULL);
}

static void _draw_mask(Enesim_Renderer *r, Enesim_Scanline_Mask *sl, Enesim_Surface *dst)
{
#if 0
	Fill_Color *f;
	Span_Color_Mask_Func cfnc;
	int nsl;
	int offset;
	int i;

	f = r->data;
		
	cfnc = enesim_surface_span_color_mask_func_get(dst, r->rop);
	offset = (dst->w * sl->y) + sl->x;
	cfnc(&dst->data, offset, f->color, sl->w, sl->coverages);
#endif
}

static void _draw(Enesim_Renderer *r, int type, void *sl, Enesim_Surface *dst)
{
	if (type == ENESIM_SCANLINE_ALIAS)
		_draw_alias(r, sl, dst);
	if (type == ENESIM_SCANLINE_MASK)
		_draw_mask(r, sl, dst);
}

static void _free(Enesim_Renderer *r)
{
	free(r->data);
}

static Enesim_Renderer_Func f_func = {
	.draw 	= _draw,
	.free 	= _free,
};
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * enesim_fill_color_new(void)
{
	Fill_Color *f;
	Enesim_Renderer *r;

	f = calloc(1, sizeof(Fill_Color));
	
	r = enesim_renderer_new();
	r->data = f;
	r->funcs = &f_func;

	return r;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_fill_color_color_set(Enesim_Renderer *r, DATA32 color)
{
	Fill_Color *f;

	assert(r);
	f = r->data;
	f->color = color;
}
