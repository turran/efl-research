#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba2_to_16bpp_rgb_565_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_0();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;
#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif
   CONVERT_LOOP2_INC_ROT_0();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;
#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 27) | (g2 << 21) | (b2 << 16) |
     (r1 << 11) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 27) | (g1 << 21) | (b1 << 16) |
     (r2 << 11) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba_to_16bpp_rgb_565_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_0();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 11) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_180();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_180();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 27) | (g2 << 21) | (b2 << 16) |
     (r1 << 11) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 27) | (g1 << 21) | (b1 << 16) |
     (r2 << 11) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba_to_16bpp_rgb_565_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_180();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 11) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_270();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_270();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 27) | (g2 << 21) | (b2 << 16) |
     (r1 << 11) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 27) | (g1 << 21) | (b1 << 16) |
     (r2 << 11) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba_to_16bpp_rgb_565_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_270();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 11) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_90();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_90();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 27) | (g2 << 21) | (b2 << 16) |
     (r1 << 11) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 27) | (g1 << 21) | (b1 << 16) |
     (r2 << 11) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_565
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba_to_16bpp_rgb_565_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_90();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 11) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba2_to_16bpp_bgr_565_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_0();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_0();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (b2 << 27) | (g2 << 21) | (r2 << 16) |
     (b1 << 11) | (g1 << 5 ) | (r1      );
#else
   *((DATA32 *)dst_ptr) =
     (b1 << 27) | (g1 << 21) | (r1 << 16) |
     (b2 << 11) | (g2 << 5 ) | (r2      );
#endif

   CONVERT_LOOP2_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba_to_16bpp_bgr_565_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_0();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (b << 11) | (g << 5) | (r);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_180();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_180();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (b2 << 27) | (g2 << 21) | (r2 << 16) |
     (b1 << 11) | (g1 << 5 ) | (r1      );
#else
   *((DATA32 *)dst_ptr) =
     (b1 << 27) | (g1 << 21) | (r1 << 16) |
     (b2 << 11) | (g2 << 5 ) | (r2      );
#endif

   CONVERT_LOOP2_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba_to_16bpp_bgr_565_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_180();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (b << 11) | (g << 5) | (r);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_270();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_270();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (b2 << 27) | (g2 << 21) | (r2 << 16) |
     (b1 << 11) | (g1 << 5 ) | (r1      );
#else
   *((DATA32 *)dst_ptr) =
     (b1 << 27) | (g1 << 21) | (r1 << 16) |
     (b2 << 11) | (g2 << 5 ) | (r2      );
#endif

   CONVERT_LOOP2_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba_to_16bpp_bgr_565_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_270();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (b << 11) | (g << 5) | (r);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_90();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 2;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith ) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 2)) >= dith2) && (g1 < 0x3f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith ) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_90();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 2;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith ) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 2)) >= dith2) && (g2 < 0x3f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith ) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (b2 << 27) | (g2 << 21) | (r2 << 16) |
     (b1 << 11) | (g1 << 5 ) | (r1      );
#else
   *((DATA32 *)dst_ptr) =
     (b1 << 27) | (g1 << 21) | (r1 << 16) |
     (b2 << 11) | (g2 << 5 ) | (r2      );
#endif

   CONVERT_LOOP2_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_BGR_565
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba_to_16bpp_bgr_565_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith, dith2;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_90();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 2;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   dith2 = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(6);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith ) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 2)) >= dith2) && (g < 0x3f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith ) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (b << 11) | (g << 5) | (r);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba2_to_16bpp_rgb_444_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_0();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_0();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 24) | (g2 << 20) | (b2 << 16) |
     (r1 << 8 ) | (g1 << 4 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 24) | (g1 << 20) | (b1 << 16) |
     (r2 << 8 ) | (g2 << 4 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba_to_16bpp_rgb_444_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_0();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 8) | (g << 4) | (b);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_180();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_180();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 24) | (g2 << 20) | (b2 << 16) |
     (r1 << 8 ) | (g1 << 4 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 24) | (g1 << 20) | (b1 << 16) |
     (r2 << 8 ) | (g2 << 4 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba_to_16bpp_rgb_444_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_180();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 8) | (g << 4) | (b);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_270();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_270();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 24) | (g2 << 20) | (b2 << 16) |
     (r1 << 8 ) | (g1 << 4 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 24) | (g1 << 20) | (b1 << 16) |
     (r2 << 8 ) | (g2 << 4 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba_to_16bpp_rgb_444_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_270();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 8) | (g << 4) | (b);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_90();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_90();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 24) | (g2 << 20) | (b2 << 16) |
     (r1 << 8 ) | (g1 << 4 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 24) | (g1 << 20) | (b1 << 16) |
     (r2 << 8 ) | (g2 << 4 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_444
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba_to_16bpp_rgb_444_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_90();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 8) | (g << 4) | (b);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba2_to_16bpp_rgb_454645_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_0();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_0();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 28) | (g2 << 23) | (b2 << 17) |
     (r1 << 12) | (g1 << 7 ) | (b1 << 1 );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 28) | (g1 << 23) | (b1 << 17) |
     (r2 << 12) | (g2 << 7 ) | (b2 << 1 );
#endif

   CONVERT_LOOP2_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba_to_16bpp_rgb_454645_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_0();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 12) | (g << 7) | (b << 1);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_180();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_180();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 28) | (g2 << 23) | (b2 << 17) |
     (r1 << 12) | (g1 << 7 ) | (b1 << 1 );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 28) | (g1 << 23) | (b1 << 17) |
     (r2 << 12) | (g2 << 7 ) | (b2 << 1 );
#endif

   CONVERT_LOOP2_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_180();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 12) | (g << 7) | (b << 1);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif


#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_270();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_270();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 28) | (g2 << 23) | (b2 << 17) |
     (r1 << 12) | (g1 << 7 ) | (b1 << 1 );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 28) | (g1 << 23) | (b1 << 17) |
     (r2 << 12) | (g2 << 7 ) | (b2 << 1 );
#endif

   CONVERT_LOOP2_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_270();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 12) | (g << 7) | (b << 1);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_90();

   r1 = (R_VAL(src_ptr)) >> 4;
   g1 = (G_VAL(src_ptr)) >> 4;
   b1 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r1 << 4)) >= dith ) && (r1 < 0x0f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 4)) >= dith ) && (g1 < 0x0f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 4)) >= dith ) && (b1 < 0x0f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_90();

   r2 = (R_VAL(src_ptr)) >> 4;
   g2 = (G_VAL(src_ptr)) >> 4;
   b2 = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r2 << 4)) >= dith ) && (r2 < 0x0f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 4)) >= dith ) && (g2 < 0x0f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 4)) >= dith ) && (b2 < 0x0f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 28) | (g2 << 23) | (b2 << 17) |
     (r1 << 12) | (g1 << 7 ) | (b1 << 1 );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 28) | (g1 << 23) | (b1 << 17) |
     (r2 << 12) | (g2 << 7 ) | (b2 << 1 );
#endif

   CONVERT_LOOP2_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_454645
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_90();

   r = (R_VAL(src_ptr)) >> 4;
   g = (G_VAL(src_ptr)) >> 4;
   b = (B_VAL(src_ptr)) >> 4;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(4);
   if (((R_VAL(src_ptr) - (r << 4)) >= dith ) && (r < 0x0f)) r++;
   if (((G_VAL(src_ptr) - (g << 4)) >= dith ) && (g < 0x0f)) g++;
   if (((B_VAL(src_ptr) - (b << 4)) >= dith ) && (b < 0x0f)) b++;
#endif

   *dst_ptr = (r << 12) | (g << 7) | (b << 1);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba2_to_16bpp_rgb_555_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_0();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 3;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 3)) >= dith) && (g1 < 0x1f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_0();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 3;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 3)) >= dith) && (g2 < 0x1f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 26) | (g2 << 21) | (b2 << 16) |
     (r1 << 10) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 26) | (g1 << 21) | (b1 << 16) |
     (r2 << 10) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT0
void
emage_convert_rgba_to_16bpp_rgb_555_dith (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_0();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 3;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 3)) >= dith) && (g < 0x1f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 10) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_0();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_180();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 3;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 3)) >= dith) && (g1 < 0x1f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_180();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 3;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 3)) >= dith) && (g2 < 0x1f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 26) | (g2 << 21) | (b2 << 16) |
     (r1 << 10) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 26) | (g1 << 21) | (b1 << 16) |
     (r2 << 10) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT180
void
emage_convert_rgba_to_16bpp_rgb_555_dith_rot_180 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_180();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 3;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 3)) >= dith) && (g < 0x1f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 10) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_180();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_270();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 3;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 3)) >= dith) && (g1 < 0x1f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_270();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 3;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 3)) >= dith) && (g2 < 0x1f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 26) | (g2 << 21) | (b2 << 16) |
     (r1 << 10) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 26) | (g1 << 21) | (b1 << 16) |
     (r2 << 10) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT270
void
emage_convert_rgba_to_16bpp_rgb_555_dith_rot_270 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_270();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 3;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 3)) >= dith) && (g < 0x1f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 10) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_270();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r1, g1, b1;
   DATA8 r2, g2, b2;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP2_START_ROT_90();

   r1 = (R_VAL(src_ptr)) >> 3;
   g1 = (G_VAL(src_ptr)) >> 3;
   b1 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r1 << 3)) >= dith) && (r1 < 0x1f)) r1++;
   if (((G_VAL(src_ptr) - (g1 << 3)) >= dith) && (g1 < 0x1f)) g1++;
   if (((B_VAL(src_ptr) - (b1 << 3)) >= dith) && (b1 < 0x1f)) b1++;
#endif

   CONVERT_LOOP2_INC_ROT_90();

   r2 = (R_VAL(src_ptr)) >> 3;
   g2 = (G_VAL(src_ptr)) >> 3;
   b2 = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r2 << 3)) >= dith) && (r2 < 0x1f)) r2++;
   if (((G_VAL(src_ptr) - (g2 << 3)) >= dith) && (g2 < 0x1f)) g2++;
   if (((B_VAL(src_ptr) - (b2 << 3)) >= dith) && (b2 < 0x1f)) b2++;
#endif

#ifndef WORDS_BIGENDIAN
   *((DATA32 *)dst_ptr) =
     (r2 << 26) | (g2 << 21) | (b2 << 16) |
     (r1 << 10) | (g1 << 5 ) | (b1      );
#else
   *((DATA32 *)dst_ptr) =
     (r1 << 26) | (g1 << 21) | (b1 << 16) |
     (r2 << 10) | (g2 << 5 ) | (b2      );
#endif

   CONVERT_LOOP2_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

#ifdef BUILD_CONVERT_16_RGB_555
#ifdef BUILD_CONVERT_16_RGB_ROT90
void
emage_convert_rgba_to_16bpp_rgb_555_dith_rot_90 (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal)
{
   DATA32 *src_ptr;
   DATA16 *dst_ptr;
   int x, y;
   DATA8 r, g, b;
#ifndef BUILD_NO_DITHER_MASK
   DATA8 dith;
#endif

   dst_ptr = (DATA16 *)dst;

   CONVERT_LOOP_START_ROT_90();

   r = (R_VAL(src_ptr)) >> 3;
   g = (G_VAL(src_ptr)) >> 3;
   b = (B_VAL(src_ptr)) >> 3;

#ifndef BUILD_NO_DITHER_MASK
   dith = DM_TABLE[(x + dith_x) & DM_MSK][(y + dith_y) & DM_MSK] >> DM_SHF(5);
   if (((R_VAL(src_ptr) - (r << 3)) >= dith) && (r < 0x1f)) r++;
   if (((G_VAL(src_ptr) - (g << 3)) >= dith) && (g < 0x1f)) g++;
   if (((B_VAL(src_ptr) - (b << 3)) >= dith) && (b < 0x1f)) b++;
#endif

   *dst_ptr = (r << 10) | (g << 5) | (b);

   CONVERT_LOOP_END_ROT_90();
   return;
   pal = 0;
}
#endif
#endif

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Converter_Func emage_converter_rgb16_get(DATA8 *dest, int w, DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Rotation rotation)
{
#ifdef BUILD_CONVERT_16_RGB_565
	     if ((rmask == 0x0000f800) && (gmask == 0x000007e0) && (bmask == 0x0000001f))
	       {
#ifdef BUILD_CONVERT_16_RGB_ROT0
		  if (rotation == 0)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_565_dith;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_565_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_565_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_565_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_565_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_565_dith_rot_90;
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
			 return emage_convert_rgba2_to_16bpp_bgr_565_dith;
		       else
			 return emage_convert_rgba_to_16bpp_bgr_565_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_bgr_565_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_bgr_565_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_bgr_565_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_bgr_565_dith_rot_90;
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
			 return emage_convert_rgba2_to_16bpp_rgb_555_dith;
		       else
 			 return emage_convert_rgba_to_16bpp_rgb_555_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_555_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_555_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_555_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_555_dith_rot_90;
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
			 return emage_convert_rgba2_to_16bpp_rgb_444_dith;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_444_dith;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_444_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_444_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_444_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_444_dith_rot_90;
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
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith;

		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_90;
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
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith;

		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT180
		  if (rotation == 180)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_180;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_180;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT270
		  if (rotation == 270)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_270;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_270;
		    }
#endif
#ifdef BUILD_CONVERT_16_RGB_ROT90
		  if (rotation == 90)
		    {
		       if ((!(w & 0x1)) && (!((int)dest & 0x3)))
			 return emage_convert_rgba2_to_16bpp_rgb_454645_dith_rot_90;
		       else
			 return emage_convert_rgba_to_16bpp_rgb_454645_dith_rot_90;
		    }
#endif
	       }
#endif
	return NULL;
}
