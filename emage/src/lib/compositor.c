#include "Emage.h"
#include "emage_private.h"

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

}

Emage_Sl_Func *
emage_compositor_sl_color_get(DATA32 col, Emage_Surface *dst, int pixels)
{

}

Emage_Sl_Func *
emage_compositor_sl_pixel_color(Emage_Surface *src, DATA32 col, Emage_Surface *dst, int pixels)
{

}

Emage_Sl_Func *
emage_compositor_sl_mask_color(DATA32 col, Emage_Surface *dst, int pixels)
{

}

Emage_Sl_Func *
emage_compositor_sl_pixel_mask(Emage_Surface *src, Emage_Surface *dst, int pixels)
{

}

/* Points */

Emage_Pt_Func *
emage_compositor_pt_pixel_get(int src_flags, Emage_Surface *dst)
{

}

Emage_Pt_Func *
emage_compositor_pt_color_get(DATA32 col, Emage_Surface *dst)
{

}

Emage_Pt_Func *
emage_compositor_pt_pixel_color_get(int src_flags, DATA32 col, Emage_Surface *dst)
{

}

Emage_Pt_Func *
emage_compositor_pt_mask_color_get(DATA32 col, Emage_Surface *dst)
{

}

Emage_Pt_Func *
emage_compositor_pt_pixel_mask_get(int src_flags, Emage_Surface *dst)
{

}

