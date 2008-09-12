#include <stdio.h>

#include "Emage.h"
#include "png.h"
/*
 * FIX ALL OF THIS
 */
/* number to checks for magic png info */
#define PNG_BYTES_TO_CHECK 4

Eina_Bool _png_info_load(const char *file, Enesim_Surface **s)
{
	
}

Eina_Bool _png_load(const char *file, Enesim_Surface **s)
{
	FILE *f;
	int bit_depth, color_type, interlace_type;
	unsigned char buf[PNG_BYTES_TO_CHECK];
	unsigned char **lines;
	char hasa, hasg;
	int i;
	
	png_uint_32 w32, h32;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	
	Enesim_Surface *tmp;
	Enesim_Surface_Data sdata;
	Eina_Bool ret = EINA_TRUE;

	hasa = 0;
	hasg = 0;
	f = fopen(file, "rb");
	if (!f)
	{
		eina_error_set(EMAGE_ERROR_EXIST);
		return EINA_FALSE;
	}
	/* if we havent read the header before, set the header data */
	fread(buf, 1, PNG_BYTES_TO_CHECK, f);
	if (!png_check_sig(buf, PNG_BYTES_TO_CHECK))
	{
		fclose(f);
		return EINA_FALSE;
	}
	rewind(f);
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
			NULL);
	if (!png_ptr)
	{
		fclose(f);
		return EINA_FALSE;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(f);
		return EINA_FALSE;
	}
	if (setjmp(png_ptr->jmpbuf))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(f);
		return EINA_FALSE;
	}
	png_init_io(png_ptr, f);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) (&w32),
			(png_uint_32 *) (&h32), &bit_depth, &color_type,
			&interlace_type, NULL, NULL);
	
	if (*s)
	{
		int w, h;
		
		enesim_surface_size_get(*s, &w, &h);
		if ((w32 != w) || (h32 != h))
		{
			ret = EINA_FALSE;
			eina_error_set(EMAGE_ERROR_SIZE);
			goto err_setup;
		}
		if (enesim_surface_format_get(*s) != ENESIM_SURFACE_ARGB8888_UNPRE)
		{
			tmp = enesim_surface_new(ENESIM_SURFACE_ARGB8888_UNPRE, w32, h32);
			if (!tmp)
			{
				ret = EINA_FALSE;
				eina_error_set(EMAGE_ERROR_LOADER);
				goto err_setup;
			}
		}
		else
			tmp = *s;
	}
	/* create the surface in case it wasnt created before */
	else
	{
		printf("creating\n");
		*s = enesim_surface_new(ENESIM_SURFACE_ARGB8888_UNPRE, w32, h32);
		if (!*s)
		{
			ret = EINA_FALSE;
			eina_error_set(EMAGE_ERROR_LOADER);
			goto err_setup;
		}
		tmp = *s;
	}

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);
	if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		hasa = 1;
	if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		hasa = 1;
		hasg = 1;
	}
	if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY)
		hasg = 1;
	if (hasa)
		png_set_expand(png_ptr);
	/* TODO check surface format, ARGB8888 for now */
#ifdef WORDS_BIGENDIAN
	png_set_swap_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_BEFORE);
#else
	png_set_bgr(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
#endif
	/* 16bit color -> 8bit color */
	png_set_strip_16(png_ptr);
	/* pack all pixels to byte boundaires */
	png_set_packing(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);

	lines = (unsigned char **) alloca(h32 * sizeof(unsigned char *));

	if (hasg)
	{
		png_set_gray_to_rgb(png_ptr);
		if (png_get_bit_depth(png_ptr, info_ptr) < 8)
			png_set_gray_1_2_4_to_8(png_ptr);
	}
	/* setup the pointers */
	enesim_surface_data_get(tmp, &sdata);
	for (i = 0; i < h32; i++)
		lines[i] = ((unsigned char *)(sdata.argb8888_unpre.plane0)) + (i * w32
				* sizeof(uint32_t));
	png_read_image(png_ptr, lines);
	png_read_end(png_ptr, info_ptr);
	/* convert the image */ 
	if (enesim_surface_format_get(*s) != ENESIM_SURFACE_ARGB8888_UNPRE)
	{
		printf("converting\n");
		enesim_surface_convert(tmp, *s);
	}
	if (tmp != *s)
	{
		printf("deleting\n");
		enesim_surface_delete(tmp);
	}
err_setup:
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
	fclose(f);
	
	return ret;
}

Eina_Bool _png_save(void)
{
#if 0
	FILE *f;
	int num_passes = 1, pass;
	int x, y, j;
	int w, h;
	Enesim_Surface_Data esdata;
	uint32_t *ptr, *data;

	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row_ptr, png_data = NULL;
	png_color_8 sig_bit;

	f = fopen(file, "wb");
	if (!f) return;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	goto error_ptr;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	goto error_info;

	if (setjmp(png_ptr->jmpbuf))
	goto error_jmp;

	//	if (s->flags & RGBA_SURFACE_HAS_ALPHA)

	{
		enesim_surface_data_get(s, &esdata);
		enesim_surface_size_get(s, &w, &h);
		data = malloc(w * h * sizeof(uint32_t));
		if (!data)
		{
			fclose(f);
			png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
			png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
			return;
		}
		memcpy(data, esdata.argb8888_unpre.plane0, w * h * sizeof(uint32_t));
		//enesim_color_data_argb_unpremul(data, w * h);
		png_init_io(png_ptr, f);
		png_set_IHDR(png_ptr, info_ptr, w, h, 8,
				PNG_COLOR_TYPE_RGB_ALPHA, png_ptr->interlaced,
				PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

#ifdef WORDS_BIGENDIAN
		png_set_swap_alpha(png_ptr);
#else
		png_set_bgr(png_ptr);
#endif
	}
#if 0
	else
	{
		data = s->data;
		png_init_io(png_ptr, f);
		png_set_IHDR(png_ptr, info_ptr, s->w, s->h, 8,
				PNG_COLOR_TYPE_RGB, png_ptr->interlaced,
				PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_data = alloca(s->w * 3 * sizeof(char));

	}
#endif
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

		for (y = 0; y < h; y++)
		{
			// if (s->flags & RGBA_SURFACE_HAS_ALPHA)
			row_ptr = (png_bytep) ptr;
#if 0
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
#endif
			png_write_rows(png_ptr, &row_ptr, 1);
			ptr += w;
		}

	}
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
	png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
	//if (s->flags & RGBA_SURFACE_HAS_ALPHA)
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
#endif
}

static Emage_Provider _provider = {
	.name = "png",
	.load = _png_load,
};

int png_provider_init(void)
{
	return emage_provider_register(&_provider);
}

void png_provider_exit(void)
{
	emage_provider_unregister(&_provider);
}
