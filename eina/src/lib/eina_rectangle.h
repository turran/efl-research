#ifndef EINA_RECTANGLE_H_
#define EINA_RECTANGLE_H_

typedef struct _Eina_Rectangle
{
	int 	x;
	int 	y;
	int 	w;
	int 	h;
} Eina_Rectangle;

static inline int
eina_rectangle_is_empty(Eina_Rectangle *r)
{
	return ((r->w < 1) || (r->h < 1));
}


static inline void
eina_rectangle_coords_from(Eina_Rectangle *r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

#endif /*EINA_RECTANGLE_H_*/
