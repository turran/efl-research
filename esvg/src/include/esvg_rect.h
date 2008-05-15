#ifndef ESVG_RECT_H_
#define ESVG_RECT_H_

#define ESVG_RECT_CLASS       (esvg_rect_class_get())
#define ESVG_RECT(obj)       (EOBJ_OBJECT_CAST((obj), ESVG_RECT_CLASS, ESVG_Rect))
#define ESVG_IS_RECT(obj)    (EOBJ_OBJECT_CHECK_CLASS((obj), ESVG_RECT_CLASS))

typedef struct _ESVG_Rect ESVG_Rect;

struct _ESVG_Rect
{
	/* inheritance */
	ESVG_Element element;
	/* private attributes */
	ESVG_Coord x;
	ESVG_Coord y;
	ESVG_Length rx;
	ESVG_Length ry;
	ESVG_Length width;
	ESVG_Length height;
};

EAPI Eobj_Class * esvg_rect_class_get(void);
EAPI ESVG_Element * esvg_rect_new(void);
EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord *x, ESVG_Coord *y, ESVG_Length *w, ESVG_Length *h);
EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry);
EAPI void esvg_rect_del(ESVG_Rect *r);

#endif /*ESVG_RECT_H_*/
