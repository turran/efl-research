#ifndef ESVG_SHAPE_H_
#define ESVG_SHAPE_H_

#define ESVG_SHAPE_CLASS       (esvg_shape_class_get())
#define ESVG_SHAPE(obj)       (EOBJ_OBJECT_CAST((obj), ESVG_SHAPE_CLASS, ESVG_Shape))
#define ESVG_IS_SHAPE(obj)    (EOBJ_OBJECT_CHECK_CLASS((obj), ESVG_SHAPE_CLASS))

/**
 * A shape interacts with the canvas engine
 * 
 */
typedef struct _ESVG_Shape ESVG_Shape;

struct _ESVG_Shape
{
	/* inheritance */
	ESVG_Element element;
	/* attributes */
	struct 
	{
		ESVG_Attribute_Core core;
#if 0
		ESVG_Attribute_Conditional conditional;
		ESVG_Attribute_Style style;
#endif
		ESVG_Attribute_Paint paint;
#if 0
		ESVG_Attribute_Color color;
#endif
		ESVG_Attribute_Opacity opacity;
		ESVG_Attribute_Graphics graphics;
		ESVG_Attribute_Clip clip;
		ESVG_Attribute_Mask mask;
#if 0
		ESVG_Attribute_Filter filter;
		ESVG_Attribute_Graphical_Events graphical_events;
#endif
		ESVG_Attribute_Cursor cursor;
	} attributes;
	/* interface */
	/* implementation */
	Ekeko_Object *object;
	Ekeko_Object_Class *object_class;
	struct
	{
		void *context;
	} engine;
};

EAPI Eobj_Class * esvg_shape_class_get(void);

EAPI void esvg_shape_id_set(ESVG_Shape *s, char *id);
EAPI void esvg_shape_color_set(ESVG_Shape *s, ESVG_Color color);
EAPI void esvg_shape_fill_set(ESVG_Shape *s, ESVG_Color fill);
EAPI void esvg_shape_fill_rule_set(ESVG_Shape *s, ESVG_Fill_Rule fill_rule);
EAPI void esvg_shape_stroke_set(ESVG_Shape *s, ESVG_Color color);
EAPI void esvg_shape_opacity_set(ESVG_Shape *s, double opacity);
EAPI void esvg_shape_stroke_opacity_set(ESVG_Shape *s, double opacity);
EAPI void esvg_shape_fill_opacity_set(ESVG_Shape *s, double opacity);

#if 0

EAPI void esvg_rect_geometry_set(ESVG_Rect *r, const char *x, const char *y, const char *w, const char *h);

EAPI ESVG_Shape * esvg_shape_ellipse_add(ESVG *e);
EAPI ESVG_Shape * esvg_shape_polygon_add(ESVG *e);
EAPI ESVG_Shape * esvg_shape_polyline_add(ESVG *e);
EAPI ESVG_Shape * esvg_shape_circle_add(ESVG *e);
EAPI ESVG_Shape * esvg_shape_line_add(ESVG *e);
EAPI void esvg_shape_line_xy_set(ESVG_Shape *s, float x1, float y1, float x2, float y2);

EAPI void esvg_shape_translate(ESVG_Shape *s, float x, float y);
EAPI void esvg_shape_scale(ESVG_Shape *s, float w, float h);
EAPI void esvg_shape_rotate(ESVG_Shape *s, float r);

#endif

#endif /*ESVG_SHAPE_H_*/
