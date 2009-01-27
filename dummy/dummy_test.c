/*
 * dummy_test.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"

int main(void)
{
	Dummy *d;
	Dummy_Canvas *dc;
	Dummy_Rect *dr;

	dummy_init();
	d = dummy_new();
	/* canvas */
	dc = dummy_canvas_get(d);
	dummy_canvas_resize(dc, 320, 240);
	/* background */
	dr = dummy_rect_add(dc);
	dummy_rect_move(dr, 0, 0);
	dummy_rect_resize(dr, 320, 240);
	dummy_rect_color_set(dr, 0xffffffff);
	/* rect 1 */
	dr = dummy_rect_add(dc);
	dummy_rect_move(dr, 20, 20);
	dummy_rect_resize(dr, 100, 100);
	dummy_rect_color_set(dr, 0xff00ff00);
	/* rect 2 */
	dr = type_instance_new(dummy_rect_type_get());
	dummy_rect_move(dr, 120, 120);
	dummy_rect_resize(dr, 100, 100);
	dummy_rect_color_set(dr, 0xffff0000);
	object_child_append((Object *)dc, (Object *)dr);
	renderable_hide((Renderable *)dr);
	dummy_loop();
	dummy_shutdown();

	return 0;
}
