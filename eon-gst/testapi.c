#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Layout *layout;

static void paint_test(Eon_Paint *p)
{
	eon_paint_show(p);
}

static void background(void)
{
	Eon_Checker *ch;

	ch = eon_checker_new(doc);

	eon_paint_square_x_set(ch, 0);
	eon_paint_square_y_set(ch, 0);
	eon_paint_square_w_rel_set(ch, 100);
	eon_paint_square_h_rel_set(ch, 100);

	eon_checker_color1_set(ch, 0xff525252);
	eon_checker_color2_set(ch, 0xff555555);
	eon_checker_sw_set(ch, 5);
	eon_checker_sh_set(ch, 5);
	ekeko_object_child_append(layout, ch);
	eon_paint_show(ch);
}

static void paint_square_test(Eon_Paint_Square *ps)
{
	eon_paint_square_x_set(ps, 0);
	eon_paint_square_y_set(ps, 0);
#if 0
	eon_paint_square_w_rel_set(ps, 100);
	eon_paint_square_h_rel_set(ps, 100);
#else
	eon_paint_square_w_set(ps, 192);
	eon_paint_square_h_set(ps, 144);
#endif
}
static void grid_test(void)
{
	Eon_Grid *g;

	g = eon_grid_new(doc);
	eon_grid_color1_set(g, 0xffffffff);
	eon_grid_color2_set(g, 0xff0000ff);
#if 0
	eon_grid_color2_set(g, 0xff525252);
	eon_grid_color1_set(g, 0xff555555);
	eon_grid_hspace_rel_set(g, 5);
	eon_grid_vspace_rel_set(g, 5);
	eon_grid_hthick_rel_set(g, 2);
	eon_grid_vthick_rel_set(g, 2);
#else
	eon_grid_hspace_set(g, 10);
	eon_grid_vspace_set(g, 10);
	eon_grid_hthick_set(g, 1);
	eon_grid_vthick_set(g, 1);
#endif
	ekeko_object_child_append(layout, g);
	paint_square_test((Eon_Paint_Square *)g);
	paint_test((Eon_Paint *)g);
}

static void checker_test(void)
{
	Eon_Checker *ch;

	ch = eon_checker_new(doc);
	eon_checker_color1_set(ch, 0xff525252);
	eon_checker_color2_set(ch, 0xff555555);
#if 0
	eon_checker_sw_rel_set(ch, 5);
	eon_checker_sh_rel_set(ch, 5);
#else
	eon_checker_sw_set(ch, 5);
	eon_checker_sh_set(ch, 5);
#endif
	ekeko_object_child_append(layout, ch);
	paint_square_test((Eon_Paint_Square *)ch);
	paint_test((Eon_Paint *)ch);
}

static void rect_test(void)
{
	Eon_Rect *r;

	r = eon_rect_new(doc);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	paint_test((Eon_Paint *)r);
	eon_shape_fill_color_set(r, 0xffffffff);
	ekeko_object_child_append(layout, r);
}

static void stack_create(void)
{
	layout = eon_stack_new(doc);
	{
		Ekeko_Value v;

		ekeko_value_int_from(&v, 1);
		ekeko_object_property_value_set(layout, "orientation", &v);
	}
	eon_paint_square_x_set(layout, 0);
	eon_paint_square_y_set(layout, 0);
	eon_paint_square_w_rel_set(layout, 100);
	eon_paint_square_h_rel_set(layout, 100);
	ekeko_object_child_append(doc, layout);
}

static void canvas_create(void)
{
	layout = eon_canvas_new(doc);
	eon_paint_square_x_set(layout, 0);
	eon_paint_square_y_set(layout, 0);
	eon_paint_square_w_rel_set(layout, 100);
	eon_paint_square_h_rel_set(layout, 100);
	ekeko_object_child_append(doc, layout);
}

/* create the eon scene */
static void scene_create(void)
{

	doc = eon_document_new("sdl", 320, 240, "resizable");
	stack_create();
	//canvas_create();
	//rect_test();
	checker_test();
	grid_test();
	checker_test();
	/* background();*/
#if 0
	checker_test();
#endif
}

int main(int argc, char **argv)
{
	eon_init();
	scene_create();
	eon_loop();
	eon_shutdown();
	return 0;
}
