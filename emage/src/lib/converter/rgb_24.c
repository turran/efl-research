#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb24_get(DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Rotation rotation)
{
#if 0
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
#endif
	return NULL;
}
