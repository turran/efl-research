#include <stdio.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb32_get(DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Rotation rotation)
{
#if 0
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
#endif
	return NULL;
}
