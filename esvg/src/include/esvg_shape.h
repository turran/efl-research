#ifndef ESVG_SHAPE_H_
#define ESVG_SHAPE_H_

typedef struct _ESVG_Shape ESVG_Shape;

typedef enum _ESVG_Fill_Rule
{
	ESVG_FILL_RULE_NONZERO,
	ESVG_FILL_RULE_EVENODD,
} ESVG_Fill_Rule;

typedef enum _ESVG_Stroke_Line_Cap
{
	ESVG_STROKE_LINE_CAP_BUTT,
	ESVG_STROKE_LINE_CAP_ROUND,
	ESVG_STROKE_LINE_CAP_SQUARE,
} ESVG_Stroke_Line_Cap;

typedef enum _ESVG_Stroke_Line_Join
{
	ESVG_STROKE_LINE_JOIN_MITER,
	ESVG_STROKE_LINE_JOIN_ROUND,
	ESVG_STROKE_LINE_JOIN_BEVEL,
} ESVG_Stroke_Line_Join;

typedef enum _ESVG_Interpolation
{
	ESVG_INTERPOLATION_AUTO,
	ESVG_INTERPOLATION_SRGB,
	ESVG_INTERPOLATION_LINEAR_RGB,
} ESVG_Interpolation;

EAPI void esvg_shape_id_set(ESVG_Shape *s, char *id);
EAPI void esvg_shape_color_set(ESVG_Shape *s, ESVG_Color color);
EAPI void esvg_shape_fill_set(ESVG_Shape *s, ESVG_Color fill);
EAPI void esvg_shape_fill_rule_set(ESVG_Shape *s, ESVG_Fill_Rule fill_rule);
EAPI void esvg_shape_stroke_set(ESVG_Shape *s, ESVG_Color color);
EAPI void esvg_shape_opacity_set(ESVG_Shape *s, double opacity);
EAPI void esvg_shape_stroke_opacity_set(ESVG_Shape *s, double opacity);
EAPI void esvg_shape_fill_opacity_set(ESVG_Shape *s, double opacity);

typedef struct _ESVG_Rect ESVG_Rect;

EAPI ESVG_Rect * esvg_rect_add(ESVG *svg);
EAPI ESVG_Shape * esvg_rect_shape_get(ESVG_Rect *r);
EAPI void esvg_rect_geometry_set(ESVG_Rect *r, ESVG_Coord *x, ESVG_Coord *y, ESVG_Length *w, ESVG_Length *h);
EAPI void esvg_rect_rounded_set(ESVG_Rect *r, ESVG_Length rx, ESVG_Length ry);

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
