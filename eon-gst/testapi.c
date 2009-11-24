#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Canvas *canvas;

static void paint_test(Eon_Paint *p)
{
	eon_paint_show(p);
}


static void paint_square_test(Eon_Paint_Square *ps)
{
	eon_paint_square_x_set(ps, 0);
	eon_paint_square_y_set(ps, 0);
	eon_paint_square_w_rel_set(ps, 100);
	eon_paint_square_h_rel_set(ps, 100);
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
	paint_square_test((Eon_Paint_Square *)ch);
	paint_test((Eon_Paint *)ch);
	ekeko_object_child_append(canvas, ch);
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
	ekeko_object_child_append(canvas, r);
}

/* create the eon scene */
static void scene_create(void)
{

	doc = eon_document_new("sdl", 320, 240, "resizable");
	canvas = eon_canvas_new(doc);
	eon_paint_square_x_set(canvas, 0);
	eon_paint_square_y_set(canvas, 0);
	eon_paint_square_w_rel_set(canvas, 100);
	eon_paint_square_h_rel_set(canvas, 100);
	ekeko_object_child_append(doc, canvas);
	rect_test();
	checker_test();
}

int main(int argc, char **argv)
{
	eon_init();
	scene_create();
	eon_loop();
	eon_shutdown();
	return 0;
}
