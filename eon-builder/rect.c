#include "builder.h"

static void _shape_move(Shape *s, int dx, int dy)
{
	Eon_Rect *r = (Eon_Rect *)s->sh;
	Eon_Coord x, y;

	//eon_rect_x_get(r, &x);
	//eon_rect_y_get(r, &y);
	// FIX THIS
	eon_square_coords_get((Eon_Square *)r, &x, &y, NULL, NULL);
	eon_rect_x_set(r, x.final + dx);
	eon_rect_y_set(r, y.final + dy);
}

static void _control_move(Control *cp, int dx, int dy)
{
	Eon_Rect *r;
	Eon_Coord x, y, w, h;

	r = (Eon_Rect *)cp->ref->sh;
	eon_square_coords_get((Eon_Square *)r, &x, &y, &w, &h);
	switch (cp->pos)
	{
		case TOP_MIDDLE:
		if (h.final - dy > 0)
		{
			eon_rect_y_set(r, y.final + dy);
			eon_rect_h_set(r, h.final - dy);
		}
		break;

		case BOTTOM_MIDDLE:
		if (h.final + dy > 0)
			eon_rect_h_set(r, h.final + dy);
		break;

		case BOTTOM_LEFT:
		if (h.final + dy > 0)
			eon_rect_h_set(r, h.final + dy);
		if (w.final - dx > 0)
		{
			eon_rect_x_set(r, x.final + dx);
			eon_rect_w_set(r, w.final - dx);
		}
		break;

		case BOTTOM_RIGHT:
		if (w.final + dx > 0)
			eon_rect_w_set(r, w.final + dx);
		if (h.final + dy > 0)
			eon_rect_h_set(r, h.final + dy);
		break;

		case TOP_RIGHT:
		if (h.final - dy > 0)
		{
			eon_rect_y_set(r, y.final + dy);
			eon_rect_h_set(r, h.final - dy);
		}
		if (w.final + dx > 0)
			eon_rect_w_set(r, w.final + dx);
		break;

		case TOP_LEFT:
		if (h.final - dy > 0)
		{
			eon_rect_y_set(r, y.final + dy);
			eon_rect_h_set(r, h.final - dy);
		}
		if (w.final - dx > 0)
		{
			eon_rect_x_set(r, x.final + dx);
			eon_rect_w_set(r, w.final - dx);
		}
		break;
	}
}

Shape * rect_new(Scene *sc)
{
	Eon_Canvas *canvas;
	Eon_Rect *r;

	canvas = sc->canvas;
	r = eon_rect_new(canvas);
	//eon_rect_color_set(r, 0xaaff0000);
	eon_rect_x_set(r, 50);
	eon_rect_y_set(r, 50);
	eon_rect_w_set(r, 150);
	eon_rect_h_set(r, 150);
	eon_rect_corner_radius_set(r, 10);
	eon_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	eon_shape_stroke_width_set(r, 2);
	eon_shape_stroke_color_set(r, 0xff000000);
	eon_rect_show(r);
	{
		Enesim_Matrix m;

		enesim_matrix_rotate(&m, 0.3);
		//enesim_matrix_scale(&m, 2, 2);
		eon_shape_matrix_set(r, &m);
	}
	{
		image_set(canvas, r);
		//checker_set(canvas, r);
	}
	return shape_new((Eon_Shape *)r, sc, _control_move, _shape_move);
}

