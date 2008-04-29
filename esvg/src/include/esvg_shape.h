#ifndef ESVG_SHAPE_H_
#define ESVG_SHAPE_H_

typedef struct _ESVG_Shape ESVG_Shape;
typedef struct _ESVG_Rect ESVG_Rect;

EAPI ESVG_Rect * esvg_rect_add(ESVG *svg);
EAPI ESVG_Shape * esvg_rect_shape_get(ESVG_Rect *r);
EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord x, ESVG_Coord y, ESVG_Length width, ESVG_Length height);
EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry);

#endif /*ESVG_SHAPE_H_*/
