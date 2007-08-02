#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define DUMMY_SL_FN(name) 					\
static void 							\
dummy_sl_##name(void *src, void *mask, DATA32 col, void *dst, 	\
	int offset, int len) 					\
{								\
	PRINTF(#name " not implemented\n"); 			\
}

#define DUMMY_PT_FN(name) 					\
static void 							\
dummy_pt_##name(void *src, void *mask, DATA32 col, void *dst, 	\
	int offset) 						\
{								\
	PRINTF(#name " not implemented\n"); 			\
}

DUMMY_SL_FN(pixel)
DUMMY_SL_FN(color)
DUMMY_SL_FN(pixel_color)
DUMMY_SL_FN(mask_color)
DUMMY_SL_FN(pixel_mask)
DUMMY_PT_FN(pixel)
DUMMY_PT_FN(color)
DUMMY_PT_FN(pixel_color)
DUMMY_PT_FN(mask_color)
DUMMY_PT_FN(pixel_mask)


static Emage_Compositor comp_default = {
	.sl_pixel 	= dummy_sl_pixel,
	.sl_color 	= dummy_sl_color,
	.sl_pixel_color = dummy_sl_pixel_color,
	.sl_mask_color 	= dummy_sl_mask_color,
	.sl_pixel_mask 	= dummy_sl_pixel_mask,
	.pt_pixel 	= dummy_pt_pixel,
	.pt_color 	= dummy_pt_color,
	.pt_pixel_color = dummy_pt_pixel_color,
	.pt_mask_color 	= dummy_pt_mask_color,
	.pt_pixel_mask 	= dummy_pt_pixel_mask,
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

Emage_Compositor Emage_Compositors[EMAGE_DATA_FORMATS][EMAGE_RENDER_OPS];

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
Emage_Sl_Func 
emage_compositor_sl_pixel_get(Emage_Draw_Context *dc, Emage_Surface *src, Emage_Surface *dst)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
	return Emage_Compositors[dst->format][dc->render_op].sl_pixel;
}

Emage_Sl_Func
emage_compositor_sl_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].sl_color;
}

Emage_Sl_Func
emage_compositor_sl_pixel_color(Emage_Draw_Context *dc,
	Emage_Surface *src, Emage_Surface *dst)
{
	if (src && dst)
	{
		assert(src->format == dst->format);
	}
	return Emage_Compositors[dst->format][dc->render_op].sl_pixel_color;
}

Emage_Sl_Func
emage_compositor_sl_mask_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{

	return Emage_Compositors[dst->format][dc->render_op].sl_mask_color;
}

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
Emage_Pt_Func
emage_compositor_pt_pixel_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel;
}

Emage_Pt_Func
emage_compositor_pt_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_color;
}

Emage_Pt_Func
emage_compositor_pt_pixel_color_get(Emage_Draw_Context *dc, Emage_Surface *src, 
	Emage_Surface *dst)
{
	
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel_color;
}

Emage_Pt_Func
emage_compositor_pt_mask_color_get(Emage_Draw_Context *dc, Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_mask_color;
}

Emage_Pt_Func
emage_compositor_pt_pixel_mask_get(Emage_Draw_Context *dc, Emage_Surface *src,
	Emage_Surface *dst)
{
	assert(dst);
	return Emage_Compositors[dst->format][dc->render_op].pt_pixel_mask;
}

void dummy_sl(Emage_Surface *src, int soffset, DATA8 *mask, int moffset, DATA32 col, Emage_Surface *dst, int doffset, int len)
{
	PRINTF("not implemented\n");

}

void dummy_pt(Emage_Surface *src, int soffset, DATA8 mask, DATA32 col, Emage_Surface *dst, int doffset)
{
	PRINTF("not implemented\n");
}
