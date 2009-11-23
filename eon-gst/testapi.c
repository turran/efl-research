#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Canvas *canvas;

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
	paint_square_test((Eon_Paint_Square *)ch);
	ekeko_object_child_append(canvas, ch);
}

/* create the eon scene */
static void scene_create(void)
{
	Eon_Rect *r;

	doc = eon_document_new("sdl", 320, 240, "resizable");
	canvas = eon_canvas_new(doc);
	printf("1\n");
	eon_paint_square_x_set(canvas, 0);
	eon_paint_square_y_set(canvas, 0);
	eon_paint_square_w_rel_set(canvas, 100);
	eon_paint_square_h_rel_set(canvas, 100);
	ekeko_object_child_append(doc, canvas);
	printf("2\n");
	checker_test();
#if 0
	r = eon_rect_new(doc);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_shape_fill_color_set(r, 0xffffffff);
	eon_shape_show(r);
	ekeko_object_child_append(canvas, r);
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
