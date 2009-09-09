#include "builder.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	Eon_Document *doc;
	Eon_Canvas *canvas;
	Eon_Rect *r;
	Shape *s;
	Scene *sc;

	eon_init();
	doc = eon_document_new("sdl", 640, 480, "resizable");
        canvas = eon_document_canvas_get(doc);
	eon_canvas_x_rel_set(canvas, 0);
	eon_canvas_y_rel_set(canvas, 0);
	eon_canvas_w_rel_set(canvas, 100);
	eon_canvas_h_rel_set(canvas, 100);

	sc = scene_new(canvas);

	rect_new(sc);
	circle_new(sc);

	eon_loop();
	eon_shutdown();

	return 0;
}
