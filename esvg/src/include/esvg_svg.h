#ifndef ESVG_SVG_H_
#define ESVG_SVG_H_

#define ESVG_SVG_CLASS       (esvg_svg_class_get())
#define ESVG_SVG(obj)       (EOBJ_OBJECT_CAST((obj), ESVG_SVG_CLASS, ESVG_Svg))
#define ESVG_IS_SVG(obj)    (EOBJ_OBJECT_CHECK_CLASS((obj), ESVG_SVG_CLASS))

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
	ESVG_Engine *engine;
	Ekeko_Canvas *canvas;
	ESVG_Rect *background;
	
};

EAPI Eobj_Class * esvg_svg_class_get(void);
EAPI ESVG_Engine * esvg_svg_engine_get(ESVG_Svg *s);
EAPI void esvg_svg_engine_set(ESVG_Svg *s, ESVG_Engine *engine);
EAPI ESVG_Element * esvg_svg_new(void);
EAPI ESVG_Element * esvg_svg_element_by_id_get(ESVG_Svg *s, const char *id);

#endif /*ESVG_SVG_H_*/
