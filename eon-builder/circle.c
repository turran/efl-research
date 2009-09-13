#include "builder.h"

static void _shape_move(Shape *s, int dx, int dy)
{
	Eon_Circle *c;
	Eon_Coord x, y;

	c = (Eon_Circle *)s->sh;
	eon_circle_x_get(c, &x);
	eon_circle_y_get(c, &y);
	eon_circle_x_set(c, x.final + dx);
	eon_circle_y_set(c, y.final + dy);
}

static void _control_move(Control *cp, int dx, int dy)
{
	Eon_Circle *c;
	float r;

	c = (Eon_Circle *)cp->ref->sh;
	r = eon_circle_radius_get(c);
	switch (cp->pos)
	{
		case TOP_LEFT:
		case BOTTOM_LEFT:
		if (r - dx > 0)
			eon_circle_radius_set(c, r - dx);
		break;

		case TOP_RIGHT:
		case BOTTOM_RIGHT:
		if (r + dx > 0)
			eon_circle_radius_set(c, r + dx);
		break;
	}
}

Shape * circle_new(Scene *sc)
{
	Eon_Circle *c;
	Eon_Canvas *canvas;

	canvas = sc->canvas;
	c = eon_circle_new(canvas);
	eon_circle_radius_set(c, 30);
	eon_circle_x_set(c, 50);
	eon_circle_y_set(c, 50);
	eon_shape_fill_color_set((Eon_Shape *)c, 0xaaff0000);
	//eon_shape_stroke_color_set(c, 0xff000000);
	//eon_shape_stroke_width_set(c, 2);
	//eon_shape_draw_mode_set(c, ENESIM_SHAPE_DRAW_MODE_STROKE);
	eon_shape_show(c);
	{
		Enesim_Matrix m;

		//enesim_matrix_rotate(&m, 0.3);
		//enesim_matrix_scale(&m, 0.5, 0.5);
		enesim_matrix_scale(&m, 2, 2);
		//eon_shape_matrix_set(c, &m);
	}
	{
		//image_set(canvas, c);
		checker_set(canvas, c);
	}
	return shape_new((Eon_Shape *)c, sc, _control_move, _shape_move);
}
