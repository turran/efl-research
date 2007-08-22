#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb8_get(Emage_Converter_Pal_Mode pal_mode)
{
#if 0
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
#endif
	return NULL;
}
