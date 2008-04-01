#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#ifdef BUILD_CONVERT_24_RGB_888
void
emage_convert_rgba_to_24bpp_rgb_888(DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA8 *dst_ptr;
   int x, y;

   dst_ptr = (DATA8 *)dst;
   src_ptr = (DATA32 *)src;

   for (y = 0; y < h; y++)
     {
	for (x = 0; x < w; x++)
	  {
	     dst_ptr[0] = R_VAL(src_ptr);
	     dst_ptr[1] = G_VAL(src_ptr);
	     dst_ptr[2] = B_VAL(src_ptr);
	     src_ptr++;
	     dst_ptr+=3;
	  }
	src_ptr += src_jump;
	dst_ptr += (dst_jump * 3);
     }
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#ifdef BUILD_CONVERT_24_BGR_888
void
emage_convert_rgba_to_24bpp_bgr_888(DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA8 *dst_ptr;
   int x, y;

   dst_ptr = (DATA8 *)dst;
   src_ptr = (DATA32 *)src;

   for (y = 0; y < h; y++)
     {
	for (x = 0; x < w; x++)
	  {
	     dst_ptr[2] = R_VAL(src_ptr);
	     dst_ptr[1] = G_VAL(src_ptr);
	     dst_ptr[0] = B_VAL(src_ptr);
	     src_ptr++;
	     dst_ptr+=3;
	  }
	src_ptr += src_jump;
	dst_ptr += (dst_jump * 3);
     }
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb24_get(DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Rotation rotation)
{
#ifdef BUILD_CONVERT_24_RGB_888
	     if ((rmask == 0x00ff0000) && (gmask == 0x0000ff00) && (bmask == 0x000000ff))
	       {
		  if (rotation == 0)
		    return emage_convert_rgba_to_24bpp_rgb_888;
	       }
#endif
#ifdef BUILD_CONVERT_24_BGR_888
	     if ((rmask == 0x000000ff) && (gmask == 0x0000ff00) && (bmask == 0x00ff0000))
	       {
		  if (rotation == 0)
		    return emage_convert_rgba_to_24bpp_bgr_888;
	       }
#endif
	return NULL;
}
