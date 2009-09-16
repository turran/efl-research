#include "builder.h"

static void _paint_set(Eon_Paint *p, Eon_Canvas *c, Eon_Shape *s)
{
	//eon_paint_coordspace_set(p, EON_COORDSPACE_USER);
	eon_paint_coordspace_set(p, EON_COORDSPACE_OBJECT);
	eon_paint_x_rel_set(p, 0);
	eon_paint_y_rel_set(p, 0);
	eon_paint_w_rel_set(p, 100);
	eon_paint_h_rel_set(p, 100);
	ekeko_object_child_append(c, p);
	eon_shape_fill_paint_set(s, p);
}

void stripes_set(Eon_Canvas *c, Eon_Shape *s)
{
	Eon_Stripes *st;

	st = eon_stripes_new();
	eon_stripes_color1_set(st, 0xaaff0000);
	eon_stripes_color2_set(st, 0xaa0000ff);
	eon_stripes_thickness1_set(st, 5);
	eon_stripes_thickness2_set(st, 10);
	_paint_set(st, c, s);
}

void checker_set(Eon_Canvas *c, Eon_Shape *s)
{
	Eon_Checker *ch;

	ch = eon_checker_new();
	eon_checker_color1_set(ch, 0xaaff0000);
	eon_checker_color2_set(ch, 0xaa0000ff);
	_paint_set(ch, c, s);
}

void image_set(Eon_Canvas *c, Eon_Shape *s)
{
	Eon_Image *i;

	i = eon_image_new();
	//eon_image_file_set(i, "/home/jl/code/efl/svn/trunk/PROTO/eon/data/tiger.png");
	eon_image_file_set(i, "/home/jl/c/e/trunk/PROTO/eon/data/tiger.png");
	_paint_set(i, c, s);
}


