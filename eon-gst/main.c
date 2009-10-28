#include <Eon.h>
#include <gst/video/video.h>

int main(void)
{
	Eon_Document *doc;
	Eon_Canvas *canvas;
	Eon_Rect *r;

	/* create the scene */
	eon_init();
	doc = eon_document_new("sdl", 640, 480, "resizable");
        canvas = eon_document_canvas_get(doc);
	eon_canvas_x_rel_set(canvas, 0);
	eon_canvas_y_rel_set(canvas, 0);
	eon_canvas_w_rel_set(canvas, 100);
	eon_canvas_h_rel_set(canvas, 100);

	/* create the gst pipe */
	gst_init();
	
	/* register the sink */

	eon_shutdown();
}
