#include <stdio.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#ifdef BUILD_CONVERT_32_RGB_8888
#ifdef BUILD_CONVERT_32_RGB_ROT0
void
emage_convert_rgba_to_32bpp_rgb_8888 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
#if 0 // JL FIX THE BLITTING FIRST
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int y;
   Gfx_Func_Copy func;

   dst_ptr = (DATA32 *)dst;
   src_ptr = src;

   func = emage_draw_func_copy_get(w, 0);

   for (y = 0; y < h; y++)
     {
	func(src_ptr, dst_ptr, w);
	src_ptr += w + src_jump;
	dst_ptr += w + dst_jump;
     }
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
#endif
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGB_8888
#ifdef BUILD_CONVERT_32_RGB_ROT180
void
emage_convert_rgba_to_32bpp_rgb_8888_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_180();

   *dst_ptr = *src_ptr;

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGB_8888
#ifdef BUILD_CONVERT_32_RGB_ROT270
void
emage_convert_rgba_to_32bpp_rgb_8888_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_270();

   *dst_ptr = *src_ptr;

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGB_8888
#ifdef BUILD_CONVERT_32_RGB_ROT90
void
emage_convert_rgba_to_32bpp_rgb_8888_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_90();

   *dst_ptr = *src_ptr;

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGBX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT0
void
emage_convert_rgba_to_32bpp_rgbx_8888 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_0();

//   *dst_ptr = (R_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (B_VAL(src_ptr) << 8);
   *dst_ptr = (*src_ptr << 8);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGBX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT180
void
emage_convert_rgba_to_32bpp_rgbx_8888_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_180();

//   *dst_ptr = (R_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (B_VAL(src_ptr) << 8);
   *dst_ptr = (*src_ptr << 8);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGBX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT270
void
emage_convert_rgba_to_32bpp_rgbx_8888_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_270();

//   *dst_ptr = (R_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (B_VAL(src_ptr) << 8);
   *dst_ptr = (*src_ptr << 8);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_RGBX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT90
void
emage_convert_rgba_to_32bpp_rgbx_8888_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_90();

//   *dst_ptr = (R_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (B_VAL(src_ptr) << 8);
   *dst_ptr = (*src_ptr << 8);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGR_8888
#ifdef BUILD_CONVERT_32_RGB_ROT0
void
emage_convert_rgba_to_32bpp_bgr_8888 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_0();

   *dst_ptr = (B_VAL(src_ptr) << 16) | (G_VAL(src_ptr) << 8) | (R_VAL(src_ptr));

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGR_8888
#ifdef BUILD_CONVERT_32_RGB_ROT180
void
emage_convert_rgba_to_32bpp_bgr_8888_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_180();

   *dst_ptr = (B_VAL(src_ptr) << 16) | (G_VAL(src_ptr) << 8) | (R_VAL(src_ptr));

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGR_8888
#ifdef BUILD_CONVERT_32_RGB_ROT270
void
emage_convert_rgba_to_32bpp_bgr_8888_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_270();

   *dst_ptr = (B_VAL(src_ptr) << 16) | (G_VAL(src_ptr) << 8) | (R_VAL(src_ptr));

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGR_8888
#ifdef BUILD_CONVERT_32_RGB_ROT90
void
emage_convert_rgba_to_32bpp_bgr_8888_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_90();

   *dst_ptr = (B_VAL(src_ptr) << 16) | (G_VAL(src_ptr) << 8) | (R_VAL(src_ptr));

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGRX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT0
void
emage_convert_rgba_to_32bpp_bgrx_8888 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_0();

   *dst_ptr = (B_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (R_VAL(src_ptr) << 8);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGRX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT180
void
emage_convert_rgba_to_32bpp_bgrx_8888_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_180();

   *dst_ptr = (B_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (R_VAL(src_ptr) << 8);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGRX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT270
void
emage_convert_rgba_to_32bpp_bgrx_8888_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_270();

   *dst_ptr = (B_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (R_VAL(src_ptr) << 8);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_32_BGRX_8888
#ifdef BUILD_CONVERT_32_RGB_ROT90
void
emage_convert_rgba_to_32bpp_bgrx_8888_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA32 *dst_ptr;
   int x, y;

   dst_ptr = (DATA32 *)dst;

   CONVERT_LOOP_START_ROT_90();

   *dst_ptr = (B_VAL(src_ptr) << 24) | (G_VAL(src_ptr) << 16) | (R_VAL(src_ptr) << 8);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
   dith_x = 0;
   dith_y = 0;
}
#endif
#endif
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb32_get(DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Rotation rotation)
{
#ifdef BUILD_CONVERT_32_RGB_8888
	     if ((rmask == 0x00ff0000) && (gmask == 0x0000ff00) && (bmask == 0x000000ff))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return emage_convert_rgba_to_32bpp_rgb_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return emage_convert_rgba_to_32bpp_rgb_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return emage_convert_rgba_to_32bpp_rgb_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return emage_convert_rgba_to_32bpp_rgb_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_RGBX_8888
	     if ((rmask == 0xff000000) && (gmask == 0x00ff0000) && (bmask == 0x0000ff00))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return emage_convert_rgba_to_32bpp_rgbx_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return emage_convert_rgba_to_32bpp_rgbx_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return emage_convert_rgba_to_32bpp_rgbx_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return emage_convert_rgba_to_32bpp_rgbx_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_BGR_8888
	     if ((rmask == 0x000000ff) && (gmask == 0x0000ff00) && (bmask == 0x00ff0000))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return emage_convert_rgba_to_32bpp_bgr_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return emage_convert_rgba_to_32bpp_bgr_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return emage_convert_rgba_to_32bpp_bgr_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return emage_convert_rgba_to_32bpp_bgr_8888_rot_90;
#endif
	       }
#endif
#ifdef BUILD_CONVERT_32_BGRX_8888
	     if ((rmask == 0x0000ff00) && (gmask == 0x00ff0000) && (bmask == 0xff000000))
	       {
#ifdef BUILD_CONVERT_32_RGB_ROT0
		  if (rotation == 0)
		    return emage_convert_rgba_to_32bpp_bgrx_8888;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT180
		  if (rotation == 180)
		    return emage_convert_rgba_to_32bpp_bgrx_8888_rot_180;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT270
		  if (rotation == 270)
		    return emage_convert_rgba_to_32bpp_bgrx_8888_rot_270;
#endif
#ifdef BUILD_CONVERT_32_RGB_ROT90
		  if (rotation == 90)
		    return emage_convert_rgba_to_32bpp_bgrx_8888_rot_90;
#endif
	       }
#endif
	return NULL;
}
