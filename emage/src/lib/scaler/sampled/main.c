#include <stdlib.h>
#include <stdio.h>

#include "Emage.h"
#include "emage_private.h"


/* 
 * My algorithm is better than evas', the problem i have is that i cannot
 * make y_offsets (row_ptr) pointers, the data of a surface is abstracted :(
 * we could optmize it by making sl composite functions to have a step
 * for each passed surface (src,dst,mask) it will improve a lot the 
 * downscaling, because i wont have to add the row offset and get the point
 * there each time
 */

/*============================================================================*
 *                                 Macros                                     * 
 *============================================================================*/
 #if 0
/* In case we dont need to scale on Y axis, this is the main function */
#define NOY(xscale_init, xscale_fun) 					\
int i;									\
int soffset, doffset; 							\
int *xscale_data;							\
Emage_Sl_Func func;							\
									\
func = emage_compositor_sl_color_get(dc, dst);				\
soffset = (src->h * sr->y) + sr->x;					\
doffset = (dst->h * cr->y) + cr->x;					\
xscale_init								\
									\
for (i = 0; i < cr->h; i++)						\
{									\
	xscale_fun							\
	doffset += dst->w;						\
	soffset += src->w;						\
}									\
free(xscale_data);

/* In case we have to scale down the Y axis, this is the main function */
#define DOWNY(xscale_init, xscale_fun) 					\
int step, err, r, i, j;							\
int soffset, doffset;							\
int *xscale_data;							\
Emage_Sl_Func func;							\
									\
step = (sr->h - cr->h) / (cr->h - 1);					\
r = (sr->h - cr->h) - (step * (cr->h - 1));				\
err = r;								\
j = 0;									\
									\
/*func = emage_compositor_sl_color_get(dc, dst);				*/\
func = emage_compositor_pt_color_get(dc, dst);				\
soffset = (src->h * sr->y) + sr->x;					\
doffset = (dst->h * cr->y) + cr->x;					\
xscale_init								\
									\
/* the real step is in units of source width */				\
step *= src->w;								\
for (i = 0; i < cr->h; i++, soffset += src->w)				\
{									\
	if (err >= cr->h)						\
	{								\
		err -= cr->h;						\
		j++;							\
	}								\
	xscale_fun							\
	soffset += step;						\
	err += r;							\
	doffset += dst->w;						\
}									\
free(xscale_data);

/* In case we have to scale up the Y axis, this is the main function */
#define UPY(xscale_init, xscale_calc, xscale_do) 			\
int step, err, r, i, j; 						\
int soffset, doffset;							\
int *xscale_data;							\
int inc = 0;								\
void *func;								\
									\
step = cr->h / sr->h;							\
r = cr->h - (step * sr->h);						\
err = r;								\
j = 0;									\
									\
xscale_init								\
soffset = (src->w * sr->y) + sr->x;					\
doffset = (dst->w * cr->y) + cr->x;					\
									\
for (i = 0; i < sr->h; i++)						\
{									\
	if (err >= sr->h)						\
	{								\
		err -= sr->h;						\
		inc = 1;						\
	}								\
	/* scale on x */						\
	xscale_calc							\
	for (j = 0; j < step + inc; j++)				\
	{								\
		xscale_do						\
		doffset += dst->w;					\
	}								\
	err += r;							\
	inc = 0;							\
	soffset += src->w;						\
}									\
//free(xscale_data);

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Up_X_Data
{
	int length;
	DATA32 color;
} Up_X_Data;


static inline Emage_Sl_Func *
_upx_init(Emage_Surface *src, int s_w, Emage_Surface *dst, 
	Emage_Draw_Context *dc, Up_X_Data **data)
{
	*data = malloc(sizeof(Up_X_Data) * s_w);
	func = emage_compositor_sl_color_get(dc, dst);
	return func;
}

/* Gets the parameters needed for upscaling on x
 * note that you'll write _length_ times each pixel */
static inline void
_upx_calc(Emage_Surface *src, int s_w, int soffset, int d_w,
	Emage_Draw_Context *dc, Up_X_Data *data)
{
	int step, err, r, i, j, width;
	int *lengths;

	step = d_w / s_w;
	width = step + 1;
	r = d_w - (step * s_w);
	err = r;
	
	for (i = 0; i < s_w; i++)
	{
		/* get color from src */
		if (err >= s_w)
		{
			err -= s_w;
			data[i].length = width;
		}
		else
		{
			data[i].length = step;
		}
		emage_surface_color_get(src, &data[i].color, soffset++);
		/* in case we have the mul set, multiply now */
		emage_draw_context_color_mul_apply(dc, &data[i].color);
		err += r;
	}
}

/* In this case the returned value are the offsets of the source surface
 * where the pixels are, this could be optimized returning a scanline
 * filled with the pixels at the offsets of the source surface, but
 * might also be too much having to do a scanline bitswapping the alpha, rgb
 * etc (i.e dependant on the data format), also the macros for scaling y
 * should be gone because the body of those functions will have to get the
 * pixel or color scanline function
 */
static inline int * _downx_init(int s_w, int d_w)
{
	int step, err, r, i, j;
	int *offsets;

	offsets = malloc(sizeof(int) * d_w);
	
	step = (s_w - d_w) / (d_w - 1);
	r = (s_w - d_w) - (step * (d_w - 1));
	err = r;
	j = 0;
	for (i = 0; i < d_w; i++, j++)
	{
		if (err >= d_w)
		{
			err -= d_w;
			j++;
		}
		offsets[i] = j;
		j += step;
		err += r;
	}
	return offsets;
}

/* this can be optmizied if the downx_init also is optimized */
static inline int _downx_do(Emage_Surface *src, int soffset,
	Emage_Surface *dst, int d_w, int doffset,
	Emage_Draw_Context *dc, Emage_Sl_Func func, int *offsets)
{
	int i;
	DATA32 col;
	int tmp_off;
#if 0
	Emage_Surface *s;

	s = emage_surface_new(src->format, d_w, d
#endif	
	for (i = 0; i < d_w; i++)
	{
		tmp_off = soffset + offsets[i];
	#if 1
		emage_surface_color_get(src, &col, tmp_off);
		/* in case we have the mul set, multiply now */
		emage_draw_context_color_mul_apply(dc, &col);
		func(src, tmp_off, NULL, 0, col, dst, doffset);
	#endif
		doffset++;
	}
}

static inline void
_upx_test(Emage_Surface *src, int s_w, int soffset,
	Emage_Surface *dst, int d_w, int doffset,
	Emage_Draw_Context *dc, Emage_Sl_Func func)
{
	int step, err, r, i, j, width;
	int *lengths;
	DATA32 color;

	step = d_w / s_w;
	width = step + 1;
	r = d_w - (step * s_w);
	err = r;
	
	for (i = 0; i < s_w; i++)
	{
		int count = step;
		/* get color from src */
		if (err >= s_w)
		{
			err -= s_w;
			count++;
		}
		emage_surface_color_get(src, &color, soffset++);
		/* in case we have the mul set, multiply now */
		emage_draw_context_color_mul_apply(dc, &color);
		doffset += count;
		func(src, soffset, NULL, 0, color, dst, doffset, count);
		err += r;
	}
}

static inline int _upx_do(Emage_Surface *src, int s_w, int soffset,
	Emage_Surface *dst, int doffset,
	Emage_Draw_Context *dc, Emage_Sl_Func func, Up_X_Data *data)
{
	int i;
	DATA32 col;
	
	for (i = 0; i < s_w; i++)
	{
		func(src, soffset, NULL, 0, data[i].color, dst, doffset, data[i].length);
		soffset++;
		doffset += data[i].length;
	}
}

#endif

/* 15518338 us/2000 scales = 7759.16  us/scale
 * On a Core 2 Duo, evas algorithm is faster by ~10%
 * On a Pentium4 this is faster by ~14%
 */
static inline void _get_lengths(int s, int d, int *lengths)
{
	int step, err, r, i, j, width;
	
	step = d / s;
	width = step + 1;
	r = d - (step * s);
	err = r;
	
	for (i = 0; i < s; i++)
	{
		if (err >= s)
		{
			err -= s;
			lengths[i] = width;
		}
		else
		{
			lengths[i] = step;
		}
		err += r;
	}
}

static inline void
_upy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int *y_lengths, *x_lengths;
	int i, j;
	int soffset, doffset;
	DATA32 *colors;
	Emage_Sl_Func func;
	
	soffset = (src->w * sr->y) + sr->x;
	doffset = (dst->w * cr->y) + cr->x;
	
	/* get the x lengths */
	x_lengths = alloca(sizeof(int) * sr->w);
	y_lengths = alloca(sizeof(int) * sr->h);
	colors = alloca(sizeof(DATA32) * sr->w);

	_get_lengths(sr->w, cr->w, x_lengths);
	_get_lengths(sr->h, cr->h, y_lengths);

	func = emage_compositor_sl_color_get(dc, dst);
	for (i = 0; i < sr->h; i++)
	{
		for (j = 0; j < sr->w; j++)
		{

			emage_surface_color_get(src, &colors[j], soffset + j);
			/* in case we have the mul set, multiply now */
			emage_draw_context_color_mul_apply(dc, &colors[j]);
		}
		for (j = 0; j < y_lengths[i]; j++)
		{
			int k;
			int tmp = doffset;
			/* scale on x */
			for (k = 0; k < sr->w; k++)
			{
				func(NULL, 0, NULL, 0, colors[k], dst, tmp, x_lengths[k]);
				tmp += x_lengths[k];
			}
			doffset += dst->w;
		}
		soffset += src->w;
	}
}

/* 16160691/2000 = 8080.34  us */
#if 0

typedef struct _Up_X_Data
{
	int length;
	DATA32 color;
} Up_X_Data;

static inline void
_upx_init(int s_w, int d_w, Up_X_Data *data)
{
	int step, err, r, i, j, width;

	step = d_w / s_w;
	width = step + 1;
	r = d_w - (step * s_w);
	err = r;
	
	for (i = 0; i < s_w; i++)
	{
		if (err >= s_w)
		{
			err -= s_w;
			data[i].length = width;
		}
		else
		{
			data[i].length = step;
		}
		err += r;
	}
}


static inline void
_upy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int inc = 0;

	Up_X_Data *data;
	Emage_Sl_Func func;

	step = cr->h / sr->h;
	r = cr->h - (step * sr->h);
	err = r;
	j = 0;

	soffset = (src->w * sr->y) + sr->x;
	doffset = (dst->w * cr->y) + cr->x;
	
	/* get the x lengths */
	data = alloca(sizeof(Up_X_Data) * sr->w);
	_upx_init(sr->w, cr->w, data);
	func = emage_compositor_sl_color_get(dc, dst);

	for (i = 0; i < sr->h; i++)
	{
		if (err >= sr->h)
		{
			err -= sr->h;
			inc = 1;
		}
		/* get the scanline data */
		for (j = 0; j < sr->w; j++)
		{
			int tmp = soffset + j;

			emage_surface_color_get(src, &data[j].color, tmp);
			/* in case we have the mul set, multiply now */
			emage_draw_context_color_mul_apply(dc, &data[j].color);
		}
		/***/
		for (j = 0; j < step + inc; j++)
		{
			int k;
			int tmp = doffset;
			/* scale on x */
			for (k = 0; k < sr->w; k++)
			{
				func(NULL, NULL, NULL, 0, data[k].color, dst, tmp, data[k].length);
				tmp += data[k].length;
			}
			/**/
			doffset += dst->w;
		}
		err += r;
		inc = 0;
		soffset += src->w;
	}
}
#endif

static inline void
_upy_downx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{

}

static inline void
_downy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{

}

/* */
static inline void _get_offsets(int s, int d, int *offsets)
{
	int step, err, r, i, j;

	step = (s - d) / (d - 1);
	r = (s - d) - (step * (d - 1));
	err = r;
	j = 0;
	for (i = 0; i < d; i++, j++)
	{
		if (err >= d)
		{
			err -= d;
			j++;
		}
		offsets[i] = j;
		j += step;
		err += r;
	}
}

static inline void _get_y_offsets(int s, int s_w, int soffset, int d, int *offsets)
{
	int step, err, r, i, j;

	step = (s - d) / (d - 1);
	r = (s - d) - (step * (d - 1));
	err = r;
	
	step *= s_w;
	for (i = 0; i < d; i++, soffset += s_w)
	{
		if (err >= d)
		{
			err -= d;
			/* one line more */
			soffset += s_w;
		}
		offsets[i] = soffset;
		soffset += step;
		err += r;
	}
}
/* 4040864 us / 2000 scales = us/scale */
static inline void
_downy_downx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int *x_offsets, *y_offsets;
	Emage_Pt_Func func;
	
	func = emage_compositor_pt_color_get(dc, dst);
	soffset = (src->h * sr->y) + sr->x;
	doffset = (dst->h * cr->y) + cr->x;
	
	/* get the x lengths */
	x_offsets = alloca(sizeof(int) * cr->w);
	
	_get_offsets(sr->w, cr->w, x_offsets);
#if 0 
	y_offsets = alloca(sizeof(int) * cr->h);
	_get_y_offsets(sr->h, src->w, soffset, cr->h, y_offsets);
	/* the real step is in units of source width */
	for (i = 0; i < cr->h; i++)
	{
		for (j = 0; j < cr->w; j++)
		{
			DATA32 color;
			emage_surface_color_get(src, &color, y_offsets[i] + x_offsets[j]);
			/* in case we have the mul set, multiply now */
			emage_draw_context_color_mul_apply(dc, &color);
			func(NULL, 0, 0, color, dst, doffset + j);
		}
		doffset += dst->w;
	}
#else
	step = (sr->h - cr->h) / (cr->h - 1);
	r = (sr->h - cr->h) - (step * (cr->h - 1));
	err = r;
	
	step *= src->w;
	for (i = 0; i < cr->h; i++, soffset += src->w)
	{
		if (err >= cr->h)
		{
			err -= cr->h;
			/* one line more */
			soffset += src->w;
		}
		for (j = 0; j < cr->w; j++)
		{
			DATA32 color;
			emage_surface_color_get(src, &color, soffset + x_offsets[j]);
			/* in case we have the mul set, multiply now */
			emage_draw_context_color_mul_apply(dc, &color);
			func(NULL, 0, 0, color, dst, doffset + j);
		}
		doffset += dst->w;
		soffset += step;
		err += r;
	}
#endif
}

static inline void
_noy_downx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
}

static inline void
_noy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
}


static inline void
_noy_nox(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	/* call the nonscaler for both (i.e copy) */
	printf("TODO\n");
}

static void _sampled_func(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	/* upscaling y */
	if (cr->h > sr->h)
	{
		if (cr->w > sr->w)
			_upy_upx(src, sr, dst, dr, dc, cr);
		else if (cr->w < sr->w)
			_upy_downx(src, sr, dst, dr, dc, cr);
		else
		{
			printf("not supported yet\n");
		}
		//	_upy_nox(src, sr, dst, dr, dc, cr);*/
	}
	/* downscaling y */
	else if (cr->h < sr->h)
	{
		if (cr->w > sr->w)
		{
			_downy_upx(src, sr, dst, dr, dc, cr);
		}
		else if (cr->w < sr->w)
			_downy_downx(src, sr, dst, dr, dc, cr);
		else
		{
			printf("not supported yet\n");
		}
		//	_downy_nox(src, sr, dst, dr, dc, cr);*/
	}
	/* noscaling y */
	else
	{
		if (cr->w > sr->w)
			_noy_upx(src, sr, dst, dr, dc, cr);
		else if (cr->w < sr->w)
			_noy_downx(src, sr, dst, dr, dc, cr);
		else
		{
			printf("not supported yet\n");
		}
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void emage_scaler_sampled_init(void)
{
#ifdef BUILD_SCALE_SAMPLE
	Emage_Scalers[EMAGE_SCALER_SAMPLED].func = _sampled_func;
#endif
}
