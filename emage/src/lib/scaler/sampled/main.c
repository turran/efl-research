#include "Emage.h"
#include "emage_private.h"

#include <stdlib.h>

/* This scaler does not provide any smoothness, but it is quite fast
 * maybe we should do some macros for the downy, upy cases as the code is
 * exactly the same but using two different functions (upx, downx)
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* Gets the parameters needed for upscaling on x
 * note that you'll write _length_ times each pixel */
static inline int * _upx_init(int s_w, int d_w)
{
	int step, err, r, i, j, width;
	int *lengths;

	lengths = malloc(sizeof(int) * s_w);
	
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
			lengths[i] = width;
		}
		else
		{
			lengths[i] = step;
		}
		err += r;
	}
	return lengths;
}

/* In this case the returned value are the offsets of the source surface
 * where the pixels are, this could be optimized returning a scanline
 * filled with the pixels at the offsets of the source surface, but
 * might also be too much having to do a scanline bitswapping the alpha, rgb
 * etc (i.e dependant on the data format)
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
	
	for (i = 0; i < d_w; i++)
	{
		tmp_off = soffset + offsets[i];
		emage_surface_color_get(src, &col, tmp_off);
		/* in case we have the mul set, multiply now */
		emage_draw_context_color_mul_apply(dc, &col);
		func(src, tmp_off, NULL, 0, col, dst, doffset, 1);
		doffset++;
	}
}

static inline int _upx_do(Emage_Surface *src, int s_w, int soffset,
	Emage_Surface *dst, int doffset,
	Emage_Draw_Context *dc, Emage_Sl_Func func, int *lengths)
{
	int i;
	DATA32 col;
	
	for (i = 0; i < s_w; i++)
	{
		emage_surface_color_get(src, &col, soffset);
		/* in case we have the mul set, multiply now */
		emage_draw_context_color_mul_apply(dc, &col);
		func(src, soffset, NULL, 0, col, dst, doffset, lengths[i]);
		soffset++;
		doffset += lengths[i];
	}
}

static inline void
_upy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int *lengths;
	int inc = 0;
	Emage_Sl_Func func;

	//int k = cr->y;

	step = cr->h / sr->h;
	r = cr->h - (step * sr->h);
	err = r;
	j = 0;

	/*printf("CR %d %d %d %d\n", cr->x, cr->y, cr->w, cr->h);
	printf("SR %d %d %d %d\n", sr->x, sr->y, sr->w, sr->h);
	printf("PARAMS %d %d %d\n", step, r, err);*/

	soffset = (src->w * sr->y) + sr->x;
	doffset = (dst->w * cr->y) + cr->x;
	lengths = _upx_init(sr->w, cr->w);

	func = emage_compositor_sl_color_get(dc, dst);
	for (i = 0; i < sr->h; i++)
	{
		if (err >= sr->h)
		{
			err -= sr->h;
			inc = 1;
		}
		/* scale on x */
		for (j = 0; j < step + inc; j++)
		{
			//printf("(%d <=> %d)\n", i + sr->y, k++);
			_upx_do(src, sr->w, soffset, dst, doffset, dc,
				func, lengths);
			doffset += dst->w;
		}
		err += r;
		inc = 0;
		//printf("err = %d\n", err);
		soffset += src->w;
	}
	//printf("done\n");
	free(lengths);
}

static inline void
_upy_downx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int *offsets;
	int inc = 0;
	Emage_Sl_Func func;

	//int k = cr->y;

	step = cr->h / sr->h;
	r = cr->h - (step * sr->h);
	err = r;
	j = 0;

	/*printf("CR %d %d %d %d\n", cr->x, cr->y, cr->w, cr->h);
	printf("SR %d %d %d %d\n", sr->x, sr->y, sr->w, sr->h);
	printf("PARAMS %d %d %d\n", step, r, err);*/

	soffset = (src->w * sr->y) + sr->x;
	doffset = (dst->w * cr->y) + cr->x;
	offsets = _downx_init(sr->w, cr->w);

	func = emage_compositor_sl_color_get(dc, dst);
	for (i = 0; i < sr->h; i++)
	{
		if (err >= sr->h)
		{
			err -= sr->h;
			inc = 1;
		}
		/* scale on x */
		for (j = 0; j < step + inc; j++)
		{
			//printf("(%d <=> %d)\n", i + sr->y, k++);
			_downx_do(src, soffset, dst, cr->w, doffset, dc,
				func, offsets);
			doffset += dst->w;
		}
		err += r;
		inc = 0;
		//printf("err = %d\n", err);
		soffset += src->w;
	}
	//printf("done\n");
	free(offsets);
}

static inline void
_downy_upx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int *lengths;
	Emage_Sl_Func func;
	
	step = (sr->h - cr->h) / (cr->h - 1);
	r = (sr->h - cr->h) - (step * (cr->h - 1));
	err = r;
	j = 0;

	soffset = (src->h * sr->y) + sr->x;
	doffset = (dst->h * cr->y) + cr->x;
	lengths = _upx_init(sr->w, cr->w);

	/* the real step is in units of source width */
	step *= src->w;
	func = emage_compositor_sl_color_get(dc, dst);
	for (i = 0; i < cr->h; i++, soffset += src->w)
	{
		if (err >= cr->h)
		{
			err -= cr->h;
			j++;
		}
		_upx_do(src, sr->w, soffset, dst, doffset, dc,
			func, lengths);
		soffset += step;
		err += r;
		doffset += dst->w;
	}
	free(lengths);
}

static inline void
_downy_downx(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int step, err, r, i, j;
	int soffset, doffset;
	int *offsets;
	Emage_Sl_Func func;
	
	step = (sr->h - cr->h) / (cr->h - 1);
	r = (sr->h - cr->h) - (step * (cr->h - 1));
	err = r;
	j = 0;

	soffset = (src->h * sr->y) + sr->x;
	doffset = (dst->h * cr->y) + cr->x;
	offsets = _downx_init(sr->w, cr->w);

	/* the real step is in units of source width */
	step *= src->w;
	func = emage_compositor_sl_color_get(dc, dst);
	for (i = 0; i < cr->h; i++, soffset += src->w)
	{
		if (err >= cr->h)
		{
			err -= cr->h;
			j++;
		}
		_downx_do(src, soffset, dst, cr->w, doffset, dc,
			func, offsets);
		soffset += step;
		err += r;
		doffset += dst->w;
	}
	free(offsets);
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
#if 0
	int y, x, doffset, *offset_x, *offset_y;
	doffset = (cr->y * dst->w) + cr->x;
#endif
	/* have to check/fix the shrink code, something is bad there
	 * so for now just arbitrary values
	 */
	cr->x = 0;
	cr->y = 0;
	cr->w = 128;
	cr->h = 80;

	sr->x = 0;
	sr->y = 0;
	sr->w = 100;
	sr->h = 192;

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
		//	_upy_nox(src, sr, dst, dr, dc, cr);*/
	}
	/* noscaling y */
	else
	{
	
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
