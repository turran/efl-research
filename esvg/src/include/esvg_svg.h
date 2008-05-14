#ifndef ESVG_SVG_H_
#define ESVG_SVG_H_

#define ESVG_SVG_CLASS       (esvg_svg_class_get())
#define ESVG_SVG(obj)       (ESVG_OBJECT_CAST((obj), ESVG_SVG_CLASS, ESVG_Svg))
#define ESVG_IS_SVG(obj)    (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_SVG_CLASS))

typedef struct _ESVG_Svg ESVG_Svg;
typedef struct _ESVG_Svg_Internal ESVG_Svg_Internal;

struct _ESVG_Svg
{
	/* inheritance */
	ESVG_Container container;
	/* attributes */
	ESVG_Length_Animated x;
	ESVG_Length_Animated y;
	ESVG_Length_Animated width;
	ESVG_Length_Animated height;
	/* implementation */
	Ekeko_Canvas *canvas;
	ESVG_Rect *background;
};

EAPI ESVG_Element * esvg_svg_new(void);
EAPI ESVG_Element * esvg_svg_element_by_id_get(ESVG_Svg *s, const char *id);

#endif /*ESVG_SVG_H_*/
