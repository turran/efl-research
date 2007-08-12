#include "Emage.h"
#include "emage_private.h"

#include "main.h"

#ifdef BUILD_C
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* sl_pixel */
/* source pixels have sparse alpha */
static inline void
_sl_pixel_salpha(DATA32 *s, DATA32 *d, int l)
{
	DATA32 *e = d + l;
	while (d < e)
	{
		switch (*s & 0xff000000)
		{
			case 0:
			break;
			
			case 0xff000000:
			*d = *s;
			break;
			
			default:
			l = 256 - (*s >> 24);
			*d = *s + MUL_256(l, *d);
			break;
		}
		s++;  d++;
	}
}
/* source pixels are argb */
static inline void
_sl_pixel_argb(DATA32 *s, DATA32 *d, int l)
{
	DATA32 *e = d + l;
	while (d < e) 
	{
		l = 256 - (*s >> 24);
		*d = *s++ + MUL_256(l, *d);
		d++;
	}
}

/* sl_pixel_color */
/* source pixels are argb, color is argb */
static inline void
_sl_pixel_color_argb_argb(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* p_c_dp */
	DATA32 *e = d + l;
	while (d < e) 
	{
		DATA32 sc = MUL4_SYM(c, *s);
		l = 256 - (sc >> 24);
		*d = sc + MUL_256(l, *d);
		d++;
		s++;
	}
}

/* source pixels are rgb, color is argb */ 
static inline void
_sl_pixel_color_rgb_argb(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* pan_c_dp */
	DATA32 *e = d + l;
	l = 256 - (c >> 24);
	while (d < e)
	{
		*d = ((c & 0xff000000) + MUL3_SYM(c, *s)) + MUL_256(l, *d);
		d++;
		s++;
	}
}

/* source pixels are argb, color is rgb */ 
static inline void
_sl_pixel_color_argb_rgb(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* p_can_dp */
	DATA32 *e = d + l;
	while (d < e)
	{
		l = 256 - (*s >> 24);
		*d = ((*s & 0xff000000) + MUL3_SYM(c, *s)) + MUL_256(l, *d);
		d++;
		s++;
	}
}

/* source pixels and color are rgb */
static inline void
_sl_pixel_color_rgb_rgb(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* pan_can_dp */
	DATA32 *e = d + l;
	while (d < e)
	{
		*d++ = 0xff000000 + MUL3_SYM(c, *s);
		s++;
	}
}

/* source pixels are argb and color has only alpha */
static inline void
_sl_pixel_color_argb_alpha(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* p_caa_dp */
	DATA32 *e = d + l;
	c = 1 + (c & 0xff);
	while (d < e)
	{
		DATA32 sc = MUL_256(c, *s);
		l = 256 - (sc >> 24);
		*d = sc + MUL_256(l, *d);
		d++;
		s++;
	}
}

/* source pixels and color have only aplha values */
static inline void
_sl_pixel_color_alpha_alpha(DATA32 *s, DATA32 c, DATA32 *d, int l)
{
	/* pan_caa_dp */
	DATA32 *e = d + l;
	c = 1 + (c & 0xff);
	while (d < e)
	{
		*d = INTERP_256(c, *s, *d);
		d++;
		s++;
	}
}

/* sl_mask_color */
/* mask is sparse alpha and color is argb */
static inline void
_sl_mask_color_salpha_argb(DATA8 *m, DATA32 c, DATA32 *d, int l)
{
	/* mas_c_dp */
	DATA32 *e = d + l;
	l = 256 - (c >> 24);
	while (d < e)
	{
		DATA32 a = *m;
		switch(a)
		{
			case 0:
			break;
			
			case 255:
			*d = c + MUL_256(l, *d);
			break;
			
			default:
			{
				DATA32 mc = MUL_SYM(a, c);
				a = 256 - (mc >> 24);
				*d = mc + MUL_256(a, *d);
			}
			break;
		}
		m++;  d++;
	}
}

/* mask is sparse alpha and color is rgb */
static inline void
_sl_mask_color_salpha_rgb(DATA8 *m, DATA32 c, DATA32 *d, int l)
{
	/* mas_can_dp */
	DATA32 *e = d + l;
	while (d < e)
	{
		l = *m;
		switch(l)
		{
			case 0:
			break;

			case 255:
			*d = c;
			break;
			
			default:
			l++;
			*d = INTERP_256(l, c, *d);
			break;
		}
		m++;  d++;
	}
}
/* _sl_pixel_mask */
static inline void
_sl_pixel_mask_argb_salpha(DATA32 *s, DATA8 *m, DATA32 *d, int l)
{
	DATA32 c;
	DATA32 *e = d + l;
	/* p_mas_dp */
	while (d < e) {
		l = *m;
		switch(l)
		{
			case 0:
			break;
			
			case 255:
			l = 256 - (*s >> 24);
			*d = *s + MUL_256(l, *d);
			break;
			
			default:
			c = MUL_SYM(l, *s);
			l = 256 - (c >> 24);
			*d = c + MUL_256(l, *d);
			break;
		}
		m++;  s++;  d++;
	}
}

static inline void
_sl_pixel_mask_salpha_salpha(DATA32 *s, DATA8 *m, DATA32 *d, int l)
{
	/* pas_mas_dp */
	DATA32 *e = d + l;
	DATA32 c;
	while (d < e) 
	{
		l = (*s >> 24);
		switch(*m & l)
		{
			case 0:
			break;
			
			case 255:
			*d = *s;
			break;
			
			default:
			c = MUL_SYM(l, *s);
			l = 256 - (c >> 24);
			*d = c + MUL_256(l, *d);
			break;
		}
		m++;  s++;  d++;
	}
}

static inline void
_sl_pixel_mask_rgb_salpha(DATA32 *s, DATA8 *m, DATA32 *d, int l)
{
	/* pan_mas_dp */
	DATA32 *e = d + l;
	while (d < e) {
		l = *m;
		switch(l)
		{
			case 0:
			break;
			case 255:
			*d = *s;
			break;
			default:
			l++;
			*d = INTERP_256(l, *s, *d);
			break;
		}
		m++;  s++;  d++;
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
SL_FUNC(blend, argb8888, c, pixel)
{
	DATA32 *s, *d;

	s = src->data.argb8888.data + soffset;
	d = dst->data.argb8888.data + doffset;

	if (src->flags & SURFACE_FLAG_HAS_SPARSE_ALPHA)
	{
		_sl_pixel_salpha(s, d, len);
	}
	else
		_sl_pixel_argb(s, d, len);
}

SL_FUNC(blend, argb8888, c, color)
{
	DATA32 *d;
	DATA32 *e, a;

	d = dst->data.argb8888.data + doffset;
	e = d + len;
	a = 256 - (col >> 24);
	
	while (d < e)
	{
		*d = col + MUL_256(a, *d);
		d++;
	}
}

SL_FUNC(blend, argb8888, c, pixel_color)
{
	DATA8 alpha;
	DATA32 *s, *d;

	s = src->data.argb8888.data + soffset;
	d = dst->data.argb8888.data + doffset;
	alpha = col >> 24;

	if (src->flags & SURFACE_FLAG_HAS_ALPHA)
	{
		if (col == ((alpha * 0x01010101)))
			_sl_pixel_color_argb_alpha(s, col, d, len);
		else if (alpha < 255)
			_sl_pixel_color_argb_argb(s, col, d, len);
		else
			_sl_pixel_color_argb_rgb(s, col, d, len);
	}
	else
	{
		if (alpha < 255)
			_sl_pixel_color_rgb_argb(s, col, d, len);
		else
			_sl_pixel_color_rgb_rgb(s, col, d, len);
	}
}

SL_FUNC(blend, argb8888, c, mask_color)
{
	DATA8 alpha;
	DATA32 *d;

	d = dst->data.argb8888.data + doffset;
	alpha = col >> 24;

	if (alpha < 255)
		_sl_mask_color_salpha_argb(mask, col, d, len);
	else
		_sl_mask_color_salpha_rgb(mask, col, d, len);
}

SL_FUNC(blend, argb8888, c, pixel_mask)
{
	DATA32 *s, *d;
	DATA8 alpha = col >> 24;

	s = src->data.argb8888.data + soffset;
	d = dst->data.argb8888.data + doffset;

	if (src->flags & SURFACE_FLAG_HAS_SPARSE_ALPHA)
	{
		_sl_pixel_mask_salpha_salpha(s, mask, d, len);
	}
	else if (alpha == 255)
	{
		_sl_pixel_mask_rgb_salpha(s, mask, d, len);
	}
	else
		_sl_pixel_mask_argb_salpha(s, mask, d, len);
}

PT_FUNC(blend, argb8888, c, pixel)
{
	DATA32 s, *d;

	s = *(src->data.argb8888.data + soffset);
	d = dst->data.argb8888.data + doffset;

	col = 256 - (s >> 24);
	*d = s + MUL_256(col, *d);
}

PT_FUNC(blend, argb8888, c, color)
{
	DATA32 s, *d;

	s = *(src->data.argb8888.data + soffset);
	d = dst->data.argb8888.data + doffset;

	s = 256 - (col >> 24);
	*d = col + MUL_256(s, *d);
}

PT_FUNC(blend, argb8888, c, pixel_color)
{
	DATA32 s, *d;

	s = *(src->data.argb8888.data + soffset);
	d = dst->data.argb8888.data + doffset;
	
	s = MUL4_SYM(col, s);
	col = 256 - (s >> 24);
	*d = s + MUL_256(col, *d);
}

PT_FUNC(blend, argb8888, c, mask_color)
{
	DATA32 s, *d;
	DATA8 alpha = col >> 24;

	d = dst->data.argb8888.data + doffset;

	if (alpha < 255)
	{
		/* mas_c_dp */
		s = MUL_SYM(mask, col);
		mask = 256 - (s >> 24);
		*d = s + MUL_256(mask, *d);
	}
	else
	{
		/* mas_can_dp */
		*d = INTERP_256(mask + 1, col, *d);
	}

}

PT_FUNC(blend, argb8888, c, pixel_mask)
{
	DATA32 s, *d;
	Emage_Data_ARGB8888 *f;

	s = *(src->data.argb8888.data + soffset);
	d = dst->data.argb8888.data + doffset;
	
	if (src->flags & SURFACE_FLAG_HAS_SPARSE_ALPHA)
	{
		/* p_mas_dp */
		s = MUL_SYM(mask, s);
		col = 256 - (s >> 24);
		*d = s + MUL_256(col, *d);
	}
	else
	{
		/* pan_mas_dp */
		*d = INTERP_256(mask + 1, s, *d);
	}
}

#else

/*
#define SL_NAME(blend, argb8888, c, pixel) dummy_sl
#define SL_NAME(blend, argb8888, c, color) dummy_sl
#define SL_NAME(blend, argb8888, c, pixel_color) dummy_sl
#define SL_NAME(blend, argb8888, c, mask_color) dummy_sl
#define SL_NAME(blend, argb8888, c, pixel_mask) dummy_sl

#define PT_NAME(blend, argb8888, c, pixel) dummy_pt
#define PT_NAME(blend, argb8888, c, color) dummy_pt
#define PT_NAME(blend, argb8888, c, pixel_color) dummy_pt
#define PT_NAME(blend, argb8888, c, mask_color) dummy_pt
#define PT_NAME(blend, argb8888, c, pixel_mask) dummy_pt
*/
#endif
