#ifndef ESVG_RECT_H_
#define ESVG_RECT_H_

typedef struct _ESVG_Rect ESVG_Rect;

struct _ESVG_Rect
{
	/* inheritance */
	ESVG_Shape shape;
	/* private attributes */
	ESVG_Coord x;
	ESVG_Coord y;
	ESVG_Length rx;
	ESVG_Length ry;
	ESVG_Length width;
	ESVG_Length height;
};

//EAPI ESVG_Rect * esvg_rect_add(ESVG *svg);
//EAPI ESVG_Rect * esvg_rect_add(ESVG_Container *container);
EAPI ESVG_Shape * esvg_rect_shape_get(ESVG_Rect *r);
EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord *x, ESVG_Coord *y, ESVG_Length *w, ESVG_Length *h);
EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry);
EAPI void esvg_rect_del(ESVG_Rect *r);

#endif /*ESVG_RECT_H_*/
