#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

#if 0
EAPI Gfx_Func_Convert
evas_common_convert_func_get(DATA8 *dest, int w, int h, int depth, DATA32 rmask, DATA32 gmask, DATA32 bmask, Convert_Pal_Mode pal_mode, int rotation)
{
   if ((rmask == 0) && (gmask == 0) && (bmask == 0))
     {
 	if (depth == 8)
	  {
#ifdef BUILD_CONVERT_8_RGB_332
	     if (pal_mode == PAL_MODE_RGB332)
	       return evas_common_convert_rgba_to_8bpp_rgb_332_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_666
	     if (pal_mode == PAL_MODE_RGB666)
	       return evas_common_convert_rgba_to_8bpp_rgb_666_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_232
	     if (pal_mode == PAL_MODE_RGB232)
	       return evas_common_convert_rgba_to_8bpp_rgb_232_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_222
	     if (pal_mode == PAL_MODE_RGB222)
	       return evas_common_convert_rgba_to_8bpp_rgb_222_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_221
	     if (pal_mode == PAL_MODE_RGB221)
	       return evas_common_convert_rgba_to_8bpp_rgb_221_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_121
	     if (pal_mode == PAL_MODE_RGB121)
	       return evas_common_convert_rgba_to_8bpp_rgb_121_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_111
	     if (pal_mode == PAL_MODE_RGB111)
	       return evas_common_convert_rgba_to_8bpp_rgb_111_dith;
#endif
	  }
 	if (depth == 1)
	  {
	  }
 	if (depth == 4)
	  {
	  }
     }
   else
     {
	if (depth == 16)
	  {
#ifdef BUILD_CONVERT_16_RGB_565
	     if ((rmask == 0x0000f800) && (gmask == 0x000007e0) && (bmask == 0x0000001f))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_565_dith;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_565_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_565_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_565_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_565_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_565_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_565_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_565_dith_rot_90;
		    }
#endif
	       }
#endif
#ifdef BUILD_CONVERT_16_BGR_565
	     if ((rmask == 0x0000001f) && (gmask == 0x000007e0) && (bmask == 0x0000f800))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_bgr_565_dith;
		       else
			 return evas_common_convert_rgba_to_16bpp_bgr_565_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_bgr_565_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_bgr_565_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_bgr_565_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_bgr_565_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_bgr_565_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_bgr_565_dith_rot_90;
		    }
#endif
	       }
#endif
#ifdef BUILD_CONVERT_16_RGB_555
	     if ((rmask == 0x00007c00) && (gmask == 0x000003e0) && (bmask == 0x0000001f))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_555_dith;
		       else
 			 return evas_common_convert_rgba_to_16bpp_rgb_555_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_555_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_555_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_555_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_555_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_555_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_555_dith_rot_90;
		    }
#endif
	       }
#endif
#ifdef BUILD_CONVERT_16_RGB_444
	     if ((rmask == 0x00000f00) && (gmask == 0x000000f0) && (bmask == 0x0000000f))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_444_dith;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_444_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_444_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_444_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_444_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_444_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_444_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_444_dith_rot_90;
		    }
#endif
	       }
#endif
#ifdef BUILD_CONVERT_16_RGB_454645
	     if ((rmask == 0x0000f000) && (gmask == 0x00000780) && (bmask == 0x0000001e))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith;

		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_90;
		    }
#endif
	       }
#endif
#ifdef BUILD_CONVERT_16_RGB_454645
	     if ((rmask == 0x0000f800) && (gmask == 0x000007e0) && (bmask == 0x0000001f))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith;

		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_180;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_270;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return evas_common_convert_rgba2_to_16bpp_rgb_454645_dith_rot_90;
		       else
			 return evas_common_convert_rgba_to_16bpp_rgb_454645_dith_rot_90;
		    }
#endif
	       }
#endif
	  }
 	if (depth == 32)
	  {
#ifdef BUILD_CONVERT_32_RGB_8888
	     if ((rmask == 0x00ff0000) && (gmask == 0x0000ff00) && (bmask == 0x000000ff))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_32bpp_rgb_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return evas_common_convert_rgba_to_32bpp_rgb_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return evas_common_convert_rgba_to_32bpp_rgb_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return evas_common_convert_rgba_to_32bpp_rgb_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_RGBX_8888
	     if ((rmask == 0xff000000) && (gmask == 0x00ff0000) && (bmask == 0x0000ff00))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_32bpp_rgbx_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return evas_common_convert_rgba_to_32bpp_rgbx_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return evas_common_convert_rgba_to_32bpp_rgbx_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return evas_common_convert_rgba_to_32bpp_rgbx_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_BGR_8888
	     if ((rmask == 0x000000ff) && (gmask == 0x0000ff00) && (bmask == 0x00ff0000))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_32bpp_bgr_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return evas_common_convert_rgba_to_32bpp_bgr_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return evas_common_convert_rgba_to_32bpp_bgr_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return evas_common_convert_rgba_to_32bpp_bgr_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_BGRX_8888
	     if ((rmask == 0x0000ff00) && (gmask == 0x00ff0000) && (bmask == 0xff000000))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_32bpp_bgrx_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return evas_common_convert_rgba_to_32bpp_bgrx_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return evas_common_convert_rgba_to_32bpp_bgrx_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return evas_common_convert_rgba_to_32bpp_bgrx_8888_rot_90;
#endif
	       }
#endif
	  }
 	if (depth == 24)
	  {
#ifdef BUILD_CONVERT_24_RGB_888
	     if ((rmask == 0x00ff0000) && (gmask == 0x0000ff00) && (bmask == 0x000000ff))
	       {
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_24bpp_rgb_888;
	       }
#endif
#ifdef BUILD_CONVERT_24_BGR_888
	     if ((rmask == 0x000000ff) && (gmask == 0x0000ff00) && (bmask == 0x00ff0000))
	       {
		  if (rotation == 0)
		    return evas_common_convert_rgba_to_24bpp_bgr_888;
	       }
#endif
	  }
	printf("depth = %i mode = %i\n", depth, pal_mode);
 	if (depth == 8)
	  {
#ifdef BUILD_CONVERT_8_RGB_332
	     if (pal_mode == PAL_MODE_RGB332)
	       return evas_common_convert_rgba_to_8bpp_rgb_332_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_666
	     if (pal_mode == PAL_MODE_RGB666)
	       return evas_common_convert_rgba_to_8bpp_rgb_666_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_232
	     if (pal_mode == PAL_MODE_RGB232)
	       return evas_common_convert_rgba_to_8bpp_rgb_232_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_222
	     if (pal_mode == PAL_MODE_RGB222)
	       return evas_common_convert_rgba_to_8bpp_rgb_222_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_221
	     if (pal_mode == PAL_MODE_RGB221)
	       return evas_common_convert_rgba_to_8bpp_rgb_221_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_121
	     if (pal_mode == PAL_MODE_RGB121)
	       return evas_common_convert_rgba_to_8bpp_rgb_121_dith;
#endif
#ifdef BUILD_CONVERT_8_RGB_111
	     if (pal_mode == PAL_MODE_RGB111)
	       return evas_common_convert_rgba_to_8bpp_rgb_111_dith;
#endif
	  }
 	if (depth == 1)
	  {
	  }
 	if (depth == 4)
	  {
	  }
    }
   /* no optimised converter for this... no generic one either. NULL */
   return NULL;
   h = 0;
   pal_mode = 0;
}
#endif

