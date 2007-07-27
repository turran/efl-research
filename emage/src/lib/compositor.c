#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void 
default_sl_pixel(Emage_Surface *src, Emage_Surface *dst, int pixels)
{

}

static void 
default_sl_pixel(Emage_Surface *src, Emage_Surface *dst, int pixels)
{

}

static void 
default_sl_color(DATA32 col, Emage_Surface *dst, int pixels)
{

}

static void 
default_sl_pixel_color(Emage_Surface *src, DATA32 col, Emage_Surface *dst, int pixels)
{

}

static void 
default_sl_mask_color(DATA32 col, Emage_Surface *dst, int pixels)
{

}

static void 
default_sl_pixel_mask(Emage_Surface *src, Emage_Surface *dst, int pixels)
{

}

/* Points */
static void
default_pt_pixel(int src_flags, Emage_Surface *dst)
{

}

static void
default_pt_color(DATA32 col, Emage_Surface *dst)
{

}

static void
default_pt_pixel_color(int src_flags, DATA32 col, Emage_Surface *dst)
{

}

static void
default_pt_mask_color(DATA32 col, Emage_Surface *dst)
{

}

static void
default_pt_pixel_mask(int src_flags, Emage_Surface *dst)
{

}

static Emage_Compositor comp_default = {
	.sl_pixel 	= default_sl_pixel,
	.sl_color 	= default_sl_color,
	.sl_pixel_color = default_sl_pixel_color,
	.sl_mask_color 	= default_sl_mask_color,
	.sl_pixel_mask 	= default_sl_pixel_mask,
	.pt_pixel 	= default_pt_pixel,
	.pt_color 	= default_pt_color,
	.pt_pixel_color = default_pt_pixel_color,
	.pt_mask_color 	= default_pt_mask_color,
	.pt_pixel_mask 	= default_pt_pixel_mask,
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

/* Set every compositor to use the default empty functions */
Emage_Compositor Emage_Compositors[EMAGE_DATA_FORMATS][EMAGE_RENDER_OP];

void emage_compositor_init(void)
{
	emage_compositor_blend_init();
	//emage_compositor_copy_init();
}


/* Scanlines */
Emage_Sl_Func  *
emage_compositor_sl_pixel_get(Emage_Surface *src, Emage_Surface *dst, int pixels)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
}

Emage_Sl_Func *
emage_compositor_sl_color_get(DATA32 col, Emage_Surface *dst, int pixels)
{
	assert(dst);

}

Emage_Sl_Func *
emage_compositor_sl_pixel_color(Emage_Surface *src, DATA32 col, Emage_Surface *dst, int pixels)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
}

Emage_Sl_Func *
emage_compositor_sl_mask_color(DATA32 col, Emage_Surface *dst, int pixels)
{

}

Emage_Sl_Func *
emage_compositor_sl_pixel_mask(Emage_Surface *src, Emage_Surface *dst, int pixels)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
}

/* Points */
Emage_Pt_Func *
emage_compositor_pt_pixel_get(Emage_Draw_Context *dc, int src_flags, 
	Emage_Surface *dst)
{
	Emage_Compositor *cp;
	assert(dst);

	cp = Emage_Compositors[dst->format][dc->op];
	return cp->pt_pixel;
}

Emage_Pt_Func *
emage_compositor_pt_color_get(Emage_Draw_Context *dc, DATA32 col, 
	Emage_Surface *dst)
{
	Emage_Compositor *cp;
	
	assert(dst);
	cp = Emage_Compositors[dst->format][dc->op];
	return cp->pt_color;
}

Emage_Pt_Func *
emage_compositor_pt_pixel_color_get(Emage_Draw_Context *dc, int src_flags, 
	DATA32 col, Emage_Surface *dst)
{
	Emage_Compositor *cp;
	
	assert(dst);
	cp = Emage_Compositors[dst->format][dc->op];
	return cp->pt_pixel_color;
}

Emage_Pt_Func *
emage_compositor_pt_mask_color_get(Emage_Draw_Context *dc, DATA32 col, 
	Emage_Surface *dst)
{
	Emage_Compositor *cp;
	
	assert(dst);
	cp = Emage_Compositors[dst->format][dc->op];
	return cp->pt_mask_color;
}

Emage_Pt_Func *
emage_compositor_pt_pixel_mask_get(Emage_Draw_Context *dc, int src_flags, 
	Emage_Surface *dst)
{
	Emage_Compositor *cp;
	
	assert(dst);
	cp = Emage_Compositors[dst->format][dc->op];
	return cp->pt_pixel_mask;
}
