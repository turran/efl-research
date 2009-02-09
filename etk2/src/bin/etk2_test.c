#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <Etk2.h>

int main(int argc, char **argv)
{
	Etk_Document *d;
	Etk_Canvas *c;
	Etk_Rect *r;

	etk_init();

	d = etk_document_new("sdl", 320, 240);
	c = etk_document_canvas_get(d);
	etk_canvas_w_rel_set(c, 100);
	etk_canvas_h_rel_set(c, 100);
	/* create a rectangle */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 0);
	etk_rect_y_rel_set(r, 0);
	etk_rect_w_rel_set(r, 100);
	etk_rect_h_rel_set(r, 100);
	ekeko_renderable_show((Ekeko_Renderable *)r);
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 25);
	etk_rect_y_rel_set(r, 25);
	etk_rect_w_rel_set(r, 50);
	etk_rect_h_rel_set(r, 50);
	ekeko_renderable_show((Ekeko_Renderable *)r);

	etk_loop();
	etk_shutdown();

	return 0;
}

