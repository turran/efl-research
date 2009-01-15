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
	dc = dummy_canvas_get(d);
	dr = dummy_rect_add(dc);
	dummy_loop(d);
	dummy_shutdown();

	return 0;
}
