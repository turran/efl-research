#include <png.h>
#include <setjmp.h>
#include <alloca.h>

#include "Emage.h"

void png_save(RGBA_Surface *s, char *file, int compress)
{
	FILE *f;
	int num_passes = 1, pass;
	int x, y, j;

	DATA32 *ptr, *data;

	png_structp         png_ptr;
	png_infop           info_ptr;
	png_bytep row_ptr, png_data = NULL;
	png_color_8 sig_bit;
	
	f = fopen(file, "wb");
	if (!f) return;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		goto error_ptr;
	
	info_ptr  = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto error_info;
	
	if (setjmp(png_ptr->jmpbuf))
		goto error_jmp;

	if (s->flags & RGBA_SURFACE_HAS_ALPHA)
	{
		data = malloc(s->w * s->h * sizeof(DATA32));
        if (!data)
          {
            fclose(f);
            png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
            png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
            return;
          }
        memcpy(data, s->data, s->w * s->h * sizeof(DATA32));
        //evas_common_convert_argb_unpremul(data, s->w * s->h);
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, s->w, s->h, 8,
                     PNG_COLOR_TYPE_RGB_ALPHA, png_ptr->interlaced,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

#ifdef WORDS_BIGENDIAN
		png_set_swap_alpha(png_ptr);
#else
		png_set_bgr(png_ptr);
#endif
	}
	else
	{
        data = s->data;
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, s->w, s->h, 8,
                     PNG_COLOR_TYPE_RGB, png_ptr->interlaced,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_data = alloca(s->w * 3 * sizeof(char));

	}
	sig_bit.red = 8;
	sig_bit.green = 8;
	sig_bit.blue = 8;
	sig_bit.alpha = 8;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);

	png_set_compression_level(png_ptr, compress);
	png_write_info(png_ptr, info_ptr);
	png_set_shift(png_ptr, &sig_bit);
	png_set_packing(png_ptr);
	for (pass = 0; pass < num_passes; pass++)
	{
ptr = data;

        for (y = 0; y < s->h; y++)
          {
             if (s->flags & RGBA_SURFACE_HAS_ALPHA)
               row_ptr = (png_bytep) ptr;
             else
               {
                  for (j = 0, x = 0; x < s->w; x++)
                    {
                       png_data[j++] = (ptr[x] >> 16) & 0xff;
                       png_data[j++] = (ptr[x] >> 8) & 0xff;
                       png_data[j++] = (ptr[x]) & 0xff;
                    }
                  row_ptr = (png_bytep) png_data;
               }
             png_write_rows(png_ptr, &row_ptr, 1);
             ptr += s->w;
          }


	}
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
	png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
	if (s->flags & RGBA_SURFACE_HAS_ALPHA)
	free(data);
	fclose(f);
	return;
error_jmp:
	png_destroy_info_struct(png_ptr, (png_infopp)&info_ptr);
error_info:
	png_destroy_write_struct(&png_ptr, (png_infopp)&info_ptr);
error_ptr:
	fclose(f);
	return;
}
