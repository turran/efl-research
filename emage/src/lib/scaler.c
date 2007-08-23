#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Emage_Scaler scaler_default = {
	.func 	= dummy_scaler
};

static inline int
_is_empty_rectangle(const Emage_Rectangle *r)
{
   return (r->w < 1) || (r->h < 1);
}

static inline void
_get_clip(const Emage_Draw_Context *dc, const Emage_Surface *s,
	  Emage_Rectangle *clip)
{
   if (dc->clip.use)
     {
	clip->x = dc->clip.r.x;
	clip->y = dc->clip.r.y;
	clip->w = dc->clip.r.w;
	clip->h = dc->clip.r.h;
	if (clip->x < 0)
	  {
	     clip->w += clip->x;
	     clip->x = 0;
	  }
	if (clip->y < 0)
	  {
	     clip->h += clip->y;
	     clip->y = 0;
	  }
	if ((clip->x + clip->w) > s->w) clip->w = s->w - clip->x;
	if ((clip->y + clip->h) > s->h) clip->h = s->h - clip->y;
     }
   else
     {
	clip->x = 0;
	clip->y = 0;
	clip->w = s->w;
	clip->h = s->h;
     }
}


static inline void
_shrink(int *s_pos, int *s_size, int pos, int size)
{
	int d;

	d = (*s_pos) - pos;
	if (d < 0)
	{
		(*s_size) += d;
		(*s_pos) = pos;
	}
	d = size + pos - (*s_pos);
	if ((*s_size) > d)
		(*s_size) = d;
}

static inline void rect_print(Emage_Rectangle *r)
{
	printf("R %d %d %d %d\n", r->x, r->y, r->w, r->h); 
}

static int
_adjust_areas(Emage_Rectangle *src,
		     int src_max_x, int src_max_y,
		     Emage_Rectangle *dst,
		     int dst_max_x, int dst_max_y,
		     Emage_Rectangle *dst_clip)
{
   if (_is_empty_rectangle(src) ||
       _is_empty_rectangle(dst) ||
       _is_empty_rectangle(dst_clip))
     return 0;

   /* shrink clip */
   _shrink(&dst_clip->x, &dst_clip->w, dst->x, dst->w);
   _shrink(&dst_clip->y, &dst_clip->h, dst->y, dst->h);
   // ok until here
rect_print(dst_clip);
   if (_is_empty_rectangle(dst_clip)) return 0;

/* sanitise x */
   if (src->x < 0)
     {
	dst->x -= (src->x * dst->w) / src->w;
	dst->w += (src->x * dst->w) / src->w;
	src->w += src->x;
	src->x = 0;
     }
   if (src->x >= src_max_x) return 0;
   if ((src->x + src->w) > src_max_x)
     {
	dst->w = (dst->w * (src_max_x - src->x)) / (src->w);
	src->w = src_max_x - src->x;
     }
   if (dst->w <= 0) return 0;
   if (src->w <= 0) return 0;
   if (dst_clip->x < 0)
     {
	dst_clip->w += dst_clip->x;
	dst_clip->x = 0;
     }
   if (dst_clip->w <= 0) return 0;
   if (dst_clip->x >= dst_max_x) return 0;

   _shrink(&dst_clip->x, &dst_clip->w, dst->x, dst_max_x);
rect_print(dst_clip);
printf("dst->x = %d, dst_max_x = %d\n", dst->x, dst_max_x);
   if (dst_clip->w <= 0) return 0;

   /* sanitise y */
   if (src->y < 0)
     {
	dst->y -= (src->y * dst->h) / src->h;
	dst->h += (src->y * dst->h) / src->h;
	src->h += src->y;
	src->y = 0;
     }
   if (src->y >= src_max_y) return 0;
   if ((src->y + src->h) > src_max_y)
     {
	dst->h = (dst->h * (src_max_y - src->y)) / (src->h);
	src->h = src_max_y - src->y;
     }
   if (dst->h <= 0) return 0;
   if (src->h <= 0) return 0;
   if (dst_clip->y < 0)
     {
	dst_clip->h += dst_clip->y;
	dst_clip->y = 0;
     }
   if (dst_clip->h <= 0) return 0;
   if (dst_clip->y >= dst_max_y) return 0;

   _shrink(&dst_clip->y, &dst_clip->h, dst->y, dst_max_y);
   if (dst_clip->h <= 0) return 0;

	rect_print(dst_clip);
	return 1;
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Scaler Emage_Scalers[EMAGE_SCALER_TYPES];

/**
 * TODO
 */
void emage_scaler_init(void)
{
	int i;

	for (i = 0; i < EMAGE_SCALER_TYPES; i++)
	{
		Emage_Scalers[i] = scaler_default;
	}
	emage_scaler_sampled_init();
	//emage_scaler_smooth_init();
}

/**
 * TODO
 */
void emage_scaler_scale(Emage_Surface *src, Emage_Rectangle *sr, Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc)
{
	Emage_Rectangle cr;

	_get_clip(dc, dst, &cr);
	if (!_adjust_areas(sr, src->w, src->h, dr, dst->w, dst->h, &cr))
		return;
	/* TODO check if we need to scale or not, if not just do a copy */
	/* call the correct scaler function */
	printf("SR %d %d %d %d\n", sr->x, sr->y, sr->w, sr->h); 
	printf("DR %d %d %d %d\n", dr->x, dr->y, dr->w, dr->h); 
	printf("CR %d %d %d %d\n", cr.x, cr.y, cr.w, cr.h); 
	Emage_Scalers[dc->stype].func(src, sr, dst, dr, dc, &cr);
}

/**
 * TODO
 */
void dummy_scaler(Emage_Surface *src, Emage_Rectangle *sr, Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc, Emage_Rectangle *cr)
{
	PRINTF("not implemented\n");
}
