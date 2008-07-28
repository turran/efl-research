#include <sys/time.h>
#include <time.h>

#include "Eina.h"
#include "Enesim.h"
#include "common.h"

#define WIDTH 1024
#define HEIGHT 1024
#define TIMES 100

const char * rop_name(Enesim_Rop rop)
{
	switch (rop)
	{
		case ENESIM_BLEND:
			return "Blend";
		case ENESIM_FILL:
			return "Fill";
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
void drawer_span_color_solid_bench(Enesim_Rop rop, Format *f)
{
	double start, end;
	int color;
	Enesim_Surface *dst;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(f->sformat, WIDTH, HEIGHT);
	color = 0xffffffff;
	dspan = enesim_drawer_span_color_get(rop, f->sformat, color);
	start = get_time();
	span_color_draw(dspan, dst, WIDTH, color);
	end = get_time();
	printf("    Span solid color [%3.3f sec]\n", end - start);
	enesim_surface_delete(dst);
}
/* drawer span transparent color bench */
void drawer_span_color_transparent_bench(Enesim_Rop rop, Format *f)
{
	double start, end;
	int color;
	Enesim_Surface *dst;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(f->sformat, WIDTH, HEIGHT);
	color = 0xff0000aa;
	dspan = enesim_drawer_span_color_get(rop, f->sformat, color);
	start = get_time();
	span_color_draw(dspan, dst, WIDTH, color);
	end = get_time();
	printf("    Span transparent color [%3.3f sec]\n", end - start);
	enesim_surface_delete(dst);
}
/* drawer span pixel bench */
void drawer_span_pixel_bench(Enesim_Rop rop, Format *f, Format *sf)
{
	double start, end;
	int color;
	Enesim_Surface *dst, *src;
	Enesim_Surface_Data ddata;
	Enesim_Drawer_Span dspan;
	
	dst = enesim_surface_new(f->sformat, WIDTH, HEIGHT);
	src = enesim_surface_new(sf->sformat, WIDTH, HEIGHT);
	dspan = enesim_drawer_span_pixel_get(rop, f->sformat, sf->sformat);
	
	start = get_time();
	span_pixel_draw(dspan, dst, WIDTH, src);
	end = get_time();
	enesim_surface_delete(dst);
	enesim_surface_delete(src);
	printf("        %s ", sf->name);
	printf("[%3.3f sec]\n",  end - start);
}

/* drawer bench */
void drawer_bench(void)
{
	int rop;
	/* TODO test all rops */
	/* TODO test all drawer functions and all formats from/to */
	printf("****************\n");
	printf("* Drawer Bench *\n");
	printf("****************\n");
	for (rop = 0; rop < ENESIM_ROPS; rop++)
	{
		int i = 0;
		Format *df = formats[0];
		
		printf("Raster operation %s\n", rop_name(rop));
		while (df)
		{
			Format *sf = formats[0];
			int j = 0;
			
			printf("%s:\n", df->name);
			
			drawer_span_color_solid_bench(rop, df);
			drawer_span_color_transparent_bench(rop, df);
			printf("    Span pixel\n");
			j = 0;
			sf = formats[0];
			while (sf)
			{
				drawer_span_pixel_bench(rop, df, sf);
				sf = formats[++j];
			}
			df = formats[++i];
		}
	}
}

int main(void)
{
	enesim_init();
	drawer_bench();
	enesim_shutdown();
}
