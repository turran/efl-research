#include "Emage.h"
#include "emage_private.h"



/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Emage_Compositor comp_default = {
	.sl_pixel 	= dummy_sl,
	.sl_color 	= dummy_sl,
	.sl_pixel_color = dummy_sl,
	.sl_mask_color 	= dummy_sl,
	.sl_pixel_mask 	= dummy_sl,
	.pt_pixel 	= dummy_pt,
	.pt_color 	= dummy_pt,
	.pt_pixel_color = dummy_pt,
	.pt_mask_color 	= dummy_pt,
	.pt_pixel_mask 	= dummy_pt,
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

Emage_Compositor Emage_Compositors[EMAGE_DATA_FORMATS][EMAGE_RENDER_OPS];

/**
 * TODO
 */
void emage_compositor_init(void)
{
	int df, rop;
	
	/* Set every compositor to use the default empty functions
	 * this should go away in case we have _all_ compositors
	 * implemented
	 */
	for (df = 0; df < EMAGE_DATA_FORMATS; df++)
		for (rop = 0; rop < EMAGE_RENDER_OPS; rop++)
			Emage_Compositors[df][rop] = comp_default;
	/* Set up the render operations compositors */
	emage_compositor_blend_init();
	//emage_compositor_copy_init();
}


/* Scanlines */
/**
 * TODO
 */
Emage_Sl_Func 
emage_compositor_sl_pixel_get(Emage_Draw_Context *dc, Emage_Surface *src, Emage_Surface *dst)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
	if (dc->mul.use)
		return Emage_Compositors[dst->format][dc->render_op].sl_pixel;
	else
		return Emage_Compositors[dst->format][dc->render_op].sl_pixel_color;
}

/**
 * TODO
 */
Emage_Sl_Func
emage_compositor_sl_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].sl_color;
}

/* TODO remove this */
/**
 * TODO
 */
Emage_Sl_Func
emage_compositor_sl_pixel_color(Emage_Draw_Context *dc,
	Emage_Surface *src, Emage_Surface *dst)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].sl_pixel_color;
}

/**
 * TODO
 */
Emage_Sl_Func
emage_compositor_sl_mask_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{

	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].sl_mask_color;
}

/**
 * TODO
 */
Emage_Sl_Func
emage_compositor_sl_pixel_mask_get(Emage_Draw_Context *dc, 
	Emage_Surface *src, Emage_Surface *dst)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
	return Emage_Compositors[dst->format][dc->render_op].sl_pixel_mask;
}

/* Points */
/**
 * TODO
 */
Emage_Pt_Func
emage_compositor_pt_pixel_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel;
}

/**
 * TODO
 */
Emage_Pt_Func
emage_compositor_pt_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].pt_color;
}

/**
 * TODO
 */
Emage_Pt_Func
emage_compositor_pt_pixel_color_get(Emage_Draw_Context *dc, Emage_Surface *src, 
	Emage_Surface *dst)
{
	
	assert(dst);
	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel_color;
}

/**
 * TODO
 */
Emage_Pt_Func
emage_compositor_pt_mask_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	// TODO use the color to setup the correct render op
	return Emage_Compositors[dst->format][dc->render_op].pt_mask_color;
}

/**
 * TODO
 */
Emage_Pt_Func
emage_compositor_pt_pixel_mask_get(Emage_Draw_Context *dc, Emage_Surface *src,
	Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel_mask;
}

/**
 * TODO
 */
void dummy_sl(Emage_Surface *src, int soffset, DATA8 *mask, int moffset, DATA32 col, Emage_Surface *dst, int doffset, int len)
{
	PRINTF("not implemented\n");
}

/**
 * TODO
 */
void dummy_pt(Emage_Surface *src, int soffset, DATA8 mask, DATA32 col, Emage_Surface *dst, int doffset)
{
	PRINTF("not implemented\n");
}
