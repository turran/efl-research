#ifndef _ESVG_H
#define _ESVG_H

#include "esvg_core.h"
#include "esvg_canvas.h"
#include "esvg_shape.h"
#include "esvg_parser.h"

/**
 * TODO split the code in two, the drawing and the parser =)
 */
#if 0


EAPI ESVG_Shape * esvg_shape_rect_add(ESVG *e);
EAPI void esvg_shape_rectangle_geometry_set(ESVG_Shape *s, Eina_Rectangle *r);
EAPI void esvg_shape_rectangle_rounding_set(ESVG_Shape *s, float rx, float ry);

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

#endif
