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
	Etk_Animation *a;
	Etk_Clock clock;
	Ekeko_Value v;
	Etk_Coord coord;

	etk_init();

	d = etk_document_new("sdl", 320, 240);
	c = etk_document_canvas_get(d);
	etk_canvas_w_rel_set(c, 100);
	etk_canvas_h_rel_set(c, 100);
	/* create the background */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 0);
	etk_rect_y_rel_set(r, 0);
	etk_rect_w_rel_set(r, 100);
	etk_rect_h_rel_set(r, 100);
	ekeko_renderable_show((Ekeko_Renderable *)r);
	/* create a rectangle */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 25);
	etk_rect_y_rel_set(r, 25);
	etk_rect_w_rel_set(r, 50);
	etk_rect_h_rel_set(r, 50);
	ekeko_renderable_show((Ekeko_Renderable *)r);

	/* let's animate something */
	a = etk_animation_new();
	ekeko_object_child_append((Ekeko_Object *)r, (Ekeko_Object *)a);
	etk_animation_property_set(a, "x");
	etk_value_coord_from(&v, &coord, 0, ETK_COORD_RELATIVE);
	etk_animation_from_set(a, &v);
	etk_value_coord_from(&v, &coord, 25, ETK_COORD_RELATIVE);
	etk_animation_to_set(a, &v);
	etk_value_clock_seconds_from(&v, &clock, 2);
	etk_animation_duration_set(a, &clock);
	etk_animation_calc_set(a, ETK_CALC_LINEAR);
	etk_animation_end_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_begin_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_repeat_set(a, -1);

	etk_loop();
	etk_shutdown();

	return 0;
}

