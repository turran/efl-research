#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

/**
 * TODO
 * add command line arguments
 */

#include "Eina.h"
#include "Enesim.h"

#include "image.h"

#define WIDTH 1024
#define HEIGHT 1024
#define TIMES 100

const char * rop_name(Enesim_Rop rop)
{
	switch (rop)
	{
		case ENESIM_BLEND:
			return "blend";
		case ENESIM_FILL:
			return "fill";
		default:
			return NULL;
	}
	return NULL;
}

double get_time(void)
{
	struct timeval      timev;

	gettimeofday(&timev, NULL);
	return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
}

void test_finish(const char *name, Enesim_Rop rop, Enesim_Surface *dst,
		Enesim_Surface *src, Enesim_Surface *mask)
{
	Enesim_Surface *img;
	Enesim_Surface_Format sfmt;
	char file[256];
	char tmp[256];
	
	sfmt = enesim_surface_format_get(dst);
	snprintf(tmp, 256, "%s_%s_%s", name, rop_name(rop), enesim_surface_format_name_get(sfmt));
	if (src)
	{
		const char tmp2[256];
		
		sfmt = enesim_surface_format_get(src);
		snprintf(tmp2, 256, "_%s", enesim_surface_format_name_get(sfmt));
		strncat(file, tmp2, 256);
	}
	if (mask)
	{
		const char tmp2[256];
		
		sfmt = enesim_surface_format_get(mask);
		snprintf(tmp2, 256, "_%s", enesim_surface_format_name_get(sfmt));
		strncat(file, tmp2, 256);
	}
	snprintf(file, 256, "%s.png", tmp);
	img = enesim_surface_new(ENESIM_SURFACE_ARGB8888, WIDTH, HEIGHT);
	enesim_surface_convert(dst, img);
	image_save(img, file, 0);
}

void span_color_draw(Enesim_Drawer_Span span, Enesim_Surface *dst, unsigned int len,
		unsigned int color)
{
	int i;
	int t;
	Enesim_Surface_Format format = enesim_surface_format_get(dst);
	
	for (t = 0; t < TIMES; t++)
	{
		Enesim_Surface_Data dtmp;
		
		enesim_surface_data_get(dst, &dtmp);
		for (i = 0; i < HEIGHT; i++)
		{
			span(&dtmp, len, NULL, color, NULL);
			enesim_surface_data_increment(&dtmp, format, len);
		}
	}
}

void span_pixel_draw(Enesim_Drawer_Span span, Enesim_Surface *dst, unsigned int len,
		Enesim_Surface *src)
{
	int i;
	int t;
	Enesim_Surface_Format dfmt = enesim_surface_format_get(dst);
	Enesim_Surface_Format sfmt = enesim_surface_format_get(src);
		
	for (t = 0; t < TIMES; t++)
	{
		Enesim_Surface_Data dtmp;
		Enesim_Surface_Data stmp;
			
		enesim_surface_data_get(dst, &dtmp);
		enesim_surface_data_get(src, &stmp);
		for (i = 0; i < HEIGHT; i++)
		{
			span(&dtmp, len, &stmp, 0, NULL);
			enesim_surface_data_increment(&dtmp, dfmt, len);
			enesim_surface_data_increment(&stmp, sfmt, len);
		}
	}
}

/* drawer span solid color bench */
void drawer_span_color_solid_bench(Enesim_Rop rop, Enesim_Surface_Format dsf)
{
	double start, end;
	int color;
	Enesim_Surface *dst;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(dsf, WIDTH, HEIGHT);
	color = 0x00ffff55;
	//color = 0x000000ff;
	dspan = enesim_drawer_span_color_get(rop, dsf, color);
	if (dspan)
	{
		start = get_time();
		span_color_draw(dspan, dst, WIDTH, color);
		end = get_time();
		printf("    Span solid color [%3.3f sec]\n", end - start);
	}
	else
	{
		printf("    Span solid color [NOT BUILT]\n");
		return;
	}
	test_finish("span_color_solid", rop, dst, NULL, NULL);
	enesim_surface_delete(dst);
}
/* drawer span transparent color bench */
void drawer_span_color_transparent_bench(Enesim_Rop rop, Enesim_Surface_Format dsf)
{
	double start, end;
	int color;
	Enesim_Surface *dst;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(dsf, WIDTH, HEIGHT);
	color = 0xff0000aa;
	dspan = enesim_drawer_span_color_get(rop, dsf, color);
	if (dspan)
	{
		start = get_time();
		span_color_draw(dspan, dst, WIDTH, color);
		end = get_time();
		printf("    Span transparent color [%3.3f sec]\n", end - start);
	}
	else
	{
		printf("    Span transparent color [NOT BUILT]\n");
		return;
	}
	test_finish("span_color_transparent", rop, dst, NULL, NULL);
	enesim_surface_delete(dst);
}
/* drawer span pixel bench */
void drawer_span_pixel_bench(Enesim_Rop rop, Enesim_Surface_Format dsf, Enesim_Surface_Format ssf)
{
	double start, end;
	int color;
	Enesim_Surface *dst, *src;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(dsf, WIDTH, HEIGHT);
	src = enesim_surface_new(ssf, WIDTH, HEIGHT);
	dspan = enesim_drawer_span_pixel_get(rop, dsf, ssf);
	if (dspan)
	{
		start = get_time();
		span_pixel_draw(dspan, dst, WIDTH, src);
		end = get_time();
		printf("        %s [%3.3f sec]\n", enesim_surface_format_name_get(ssf), end - start);
	}
	else
	{
		printf("        %s [NOT BUILT]\n", enesim_surface_format_name_get(ssf));
		return;
	}
	test_finish("span_pixel", rop, dst, src, NULL);
	enesim_surface_delete(dst);
	enesim_surface_delete(src);
}
/* transformer bench */
void transformer_bench(void)
{
	printf("*********************\n");
	printf("* Transformer Bench *\n");
	printf("*********************\n");
}
/* drawer bench */
void drawer_bench(void)
{
	int rop;
	
	/* TODO test all drawer functions and all formats from/to */
	printf("****************\n");
	printf("* Drawer Bench *\n");
	printf("****************\n");
	for (rop = 0; rop < ENESIM_ROPS; rop++)
	{
		Enesim_Surface_Format dsf;
		
		printf("operation %s\n", rop_name(rop));
		for (dsf = ENESIM_SURFACE_ARGB8888; dsf < ENESIM_SURFACE_FORMATS; dsf++)
		{
			Enesim_Surface_Format ssf;
			
			printf("%s:\n", enesim_surface_format_name_get(dsf));
			
			drawer_span_color_solid_bench(rop, dsf);
			drawer_span_color_transparent_bench(rop, dsf);
			printf("    Span pixel\n");
			for (ssf = ENESIM_SURFACE_ARGB8888; ssf < ENESIM_SURFACE_FORMATS; ssf++)
			{
				drawer_span_pixel_bench(rop, dsf, ssf);
			}
		}
	}
}

int main(void)
{
	enesim_init();
	drawer_bench();
	enesim_shutdown();
}
