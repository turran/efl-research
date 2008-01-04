#ifndef _EDATA_RECTANGLE_H_
#define _EDATA_RECTANGLE_H_

typedef struct _Edata_Rectangle
{
	int 	x;
	int 	y;
	int 	w;
	int 	h;
} Edata_Rectangle;

static inline int
edata_spans_intersect(int c1, int l1, int c2, int l2)
{
	return (!(((c2 + l2) <= c1) || (c2 >= (c1 + l1))));
}

static inline int
edata_rectangles_intersect(Edata_Rectangle *r1, Edata_Rectangle *r2)
{
	return (edata_spans_intersect(r1->x, r1->w, r2->x, r2->w) && edata_spans_intersect(r1->y, r1->h, r2->y, r2->h));
}

static inline int
edata_rectangle_hspan_inside(Edata_Rectangle *r, int x, int l)
{
	
}

static inline int
edata_rectangle_vspan_inside(Edata_Rectangle *r, int y, int l)
{
	
}

static inline int
edata_rectangle_xcoord_inside(Edata_Rectangle *r, int x)
{
	return ((x >= r->x) && (x < (r->x + r->w)));
}

static inline int
edata_rectangle_ycoord_inside(Edata_Rectangle *r, int y)
{
	return ((y >= r->y) && (y < (r->y + r->h)));
}

static inline void
edata_rectangle_rectangle_intersection_get(Edata_Rectangle *d, Edata_Rectangle *s)
{
	if (edata_rectangles_intersect(d, s))
	{
		if (d->x < s->x)
		{
			d->x = s->x;
			d->w += d->x - s->x;
			if (d->w < 0)
				d->w = 0;
		}
		if ((d->x + d->w) > (s->x + s->w))
		{
			d->w = s->x +s->w - d->x;
			if (d->y < s->y)
			{
				d->h += d->y - s->y;
				d->y = s->y;
				if (d->h < 0)
					d->h = 0;
			
			}
			if ((d->y + d->h) > (s->y + s->h))
				d->h = s->y + s->h - d->y;
		}
	}
	else
	{
		d->w = 0;
		d->h = 0;
	}
}

static inline int
edata_rectangle_is_empty(Edata_Rectangle *r)
{
	return ((r->w < 1) || (r->h < 1));
}


static inline void
edata_rectangle_coords_from(Edata_Rectangle *r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

#endif /*_EDATA_RECTANGLE_H_*/
