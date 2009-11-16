#include <Eon.h>
#include <gst/gst.h>
#include <gst/video/video.h>

Eon_Document *doc;
Eon_Canvas *canvas;
Eon_Buffer *b;

/* create the eon scene */
static void scene_create(void)
{
	Eon_Rect *r;

	doc = eon_document_new("sdl", 640, 480, "resizable");

	canvas = eon_canvas_new(doc);
	ekeko_object_child_append(doc, canvas);
	printf("1\n");
	eon_paint_square_x_rel_set(canvas, 0);
	eon_paint_square_y_rel_set(canvas, 0);
	eon_paint_square_w_rel_set(canvas, 100);
	eon_paint_square_h_rel_set(canvas, 100);

	r = eon_rect_new(doc);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_shape_fill_color_set(r, 0xffffffff);
	eon_shape_show(r);
	ekeko_object_child_append(canvas, r);

#if 0
	r = eon_rect_new(doc);
	ekeko_object_child_append(canvas, r);
	eon_rect_x_rel_set(r, 25);
	eon_rect_y_rel_set(r, 25);
	eon_rect_w_rel_set(r, 50);
	eon_rect_h_rel_set(r, 50);
	eon_rect_corner_radius_set(r, 30);
	eon_shape_stroke_width_set(r, 2);
	eon_shape_stroke_color_set(r, 0xff000000);
	eon_shape_show(r);
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
