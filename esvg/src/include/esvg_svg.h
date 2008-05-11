#ifndef ESVG_SVG_H_
#define ESVG_SVG_H_

#define ESVG_SVG_CLASS       (esvg_svg_class_get())
#define ESVG_SVG(obj)       (ESVG_OBJECT_CAST((obj), ESVG_SVG_CLASS, ESVG_Svg))
#define ESVG_IS_SVG(obj)    (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_SVG_CLASS))

typedef struct _ESVG_Svg ESVG_Svg;

struct _ESVG_Svg
{
	/* inheritance */
	ESVG_Container container;
	/* attributes */
	ESVG_Length x;
	ESVG_Length y;
	ESVG_Length width;
	ESVG_Length height;
	/* implementation */
	ESVG_Rect *background;
};

#endif /*ESVG_SVG_H_*/
