#include <stdio.h>
#include "eranoid.h"

/*
 * TODO
 * we should define somewhere the block size, so the whole application
 * is relative to that block size
 */

int main(void)
{
	Eon_Document *d;
	Eon_Canvas *c;
	Eon_Rect *bkg;
	Level *l;

	eon_init();
	d = eon_document_new("sdl", BLOCKW * MAXCOLS, BLOCKH * TOTROWS, NULL);
	c = eon_document_canvas_get(d);
	eon_canvas_w_rel_set(c, 100);
	eon_canvas_h_rel_set(c, 100);
	/* background, should be part of the level */
	bkg = eon_rect_new(c);
	eon_rect_x_set(bkg, 0);
	eon_rect_y_set(bkg, 0);
	eon_rect_w_rel_set(bkg, 100);
	eon_rect_h_rel_set(bkg, 100);
	eon_rect_color_set(bkg, 0xffffffff);
	eon_rect_rop_set(bkg, ENESIM_FILL);
	eon_rect_show(bkg);

	l = simplelevel(c);
	level_start(l);
	eon_loop();
	eon_shutdown();
	return 0;
}
