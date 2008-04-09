#ifndef _ENESIM_RECTANGLE_H_
#define _ENESIM_RECTANGLE_H_
/**
 * @file
 * @{
 */

typedef struct _Enesim_Rectangle
{
	int 	x;
	int 	y;
	int 	w;
	int 	h;
} Enesim_Rectangle;

static inline int
enesim_rectangle_is_empty(Enesim_Rectangle *r)
{
	return ((r->w < 1) || (r->h < 1));
}


static inline void
enesim_rectangle_coords_from(Enesim_Rectangle *r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_spans_intersect(int c1, int l1, int c2, int l2)
{
	return (!(((c2 + l2) <= c1) || (c2 >= (c1 + l1))));
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_rectangles_intersect(Enesim_Rectangle *r1, Enesim_Rectangle *r2)
{
	return (enesim_spans_intersect(r1->x, r1->w, r2->x, r2->w) && enesim_spans_intersect(r1->y, r1->h, r2->y, r2->h));
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_rectangle_hspan_inside(Enesim_Rectangle *r, int x, int l)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_rectangle_vspan_inside(Enesim_Rectangle *r, int y, int l)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_rectangle_xcoord_inside(Enesim_Rectangle *r, int x)
{
	return ((x >= r->x) && (x < (r->x + r->w)));
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline int
enesim_rectangle_ycoord_inside(Enesim_Rectangle *r, int y)
{
	return ((y >= r->y) && (y < (r->y + r->h)));
}
/**
 * To be documented
 * FIXME: To be fixed
 */
static inline Eina_Bool
enesim_rectangle_rectangle_intersection_get(Enesim_Rectangle *r1, Enesim_Rectangle *r2)
{
	if (!(enesim_rectangles_intersect(r1, r2)))
		return EINA_FALSE;
	
	/* left */
	if (r1->x < r2->x)
	{
		r1->w += r1->x - r2->x;
		r1->x = r2->x;
		if (r1->w < 0)
			r1->w = 0;
	}
	/* right */
	if ((r1->x + r1->w) > (r2->x + r2->w))
	{
		r1->w = r2->x +r2->w - r1->x;
	}
	/* top */
	if (r1->y < r2->y)
	{
		r1->h += r1->y - r2->y;
		r1->y = r2->y;
		if (r1->h < 0)
			r1->h = 0;
	}
	/* bottom */
	if ((r1->y + r1->h) > (r2->y + r2->h))
		r1->h = r2->y + r2->h - r1->y;
	return EINA_TRUE;
}

/**
 * Rescale the coordinates from @in as if it where relative to @out
 */
static inline void
enesim_rectangle_rescale_in(Enesim_Rectangle *out, Enesim_Rectangle *in, Enesim_Rectangle *res)
{
	res->x = in->x - out->x;
	res->y = in->y - out->y;
	res->w = in->w;
	res->h = in->h;
}
/**
 * 
 */
static inline void
enesim_rectangle_rescale_out(Enesim_Rectangle *out, Enesim_Rectangle *in, Enesim_Rectangle *res)
{
	res->x = out->x + in->x;
	res->y = out->y + in->y;
	res->w = out->w;
	res->h = out->h;
}


/** @} */
#endif /*_ENESIM_RECTANGLE_H_*/
