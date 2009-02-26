#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <Etk2.h>

int main(int argc, char **argv)
{
	Etk_Document *d;
	Etk_Canvas *c, *sc;
	Etk_Rect *r;
	Etk_Animation *a;
	Etk_Clock clock;
	Ekeko_Value v;
	Etk_Coord coord;
	Enesim_Matrix mx1, mx2;
	Etk_Image *i;

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
	etk_shape_color_set((Etk_Shape *)r, 0xffffffff);
	etk_shape_rop_set((Etk_Shape *)r, 1);
	ekeko_renderable_show((Ekeko_Renderable *)r);

	/* create an image */
	i = etk_image_new(c);
	etk_image_x_rel_set(i, 10);
	etk_image_y_rel_set(i, 10);
	etk_image_w_rel_set(i, 60);
	etk_image_h_rel_set(i, 60);
	etk_image_file_set(i, "../data/tiger.png");
	ekeko_renderable_show((Ekeko_Renderable *)i);
#if 1
	/* add a subcanvas */
	sc = etk_canvas_new(c);
	etk_canvas_x_rel_set(sc, 0);
	etk_canvas_y_rel_set(sc, 0);
	etk_canvas_w_rel_set(sc, 50);
	etk_canvas_h_rel_set(sc, 50);

	enesim_matrix_rotate(&mx1, 1.3);
	enesim_matrix_translate(&mx2, 60, 20);
	enesim_matrix_compose(&mx2, &mx1, &mx1);
	enesim_matrix_translate(&mx2, -60, -20);
	enesim_matrix_compose(&mx1, &mx2, &mx1);
	//enesim_matrix_translate(&mx1, 30, 30);
	//enesim_matrix_scale(&mx1, 0.8, 0.8);
	etk_canvas_matrix_set(sc, &mx1);
	ekeko_renderable_show((Ekeko_Renderable *)sc);
	r = etk_rect_new(sc);
	etk_rect_x_rel_set(r, 0);
	etk_rect_y_rel_set(r, 0);
	etk_rect_w_rel_set(r, 100);
	etk_rect_h_rel_set(r, 100);
	etk_shape_color_set((Etk_Shape *)r, 0xaaaa00aa);
	etk_shape_rop_set((Etk_Shape *)r, ENESIM_FILL);
	ekeko_renderable_show((Ekeko_Renderable *)r);
#endif
#if 0
	a = etk_animation_new();
	ekeko_object_child_append((Ekeko_Object *)r, (Ekeko_Object *)a);
	etk_animation_property_set(a, "w");
	etk_value_coord_from(&v, &coord, 0, ETK_COORD_RELATIVE);
	etk_animation_from_set(a, &v);
	etk_value_coord_from(&v, &coord, 100, ETK_COORD_RELATIVE);
	etk_animation_to_set(a, &v);
	etk_value_clock_seconds_from(&v, &clock, 2);
	etk_animation_duration_set(a, &clock);
	etk_animation_calc_set(a, ETK_CALC_LINEAR);
	etk_animation_end_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_begin_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_repeat_set(a, -1);
#endif
	/* create a rectangle */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 25);
	etk_rect_y_rel_set(r, 25);
	etk_rect_w_rel_set(r, 50);
	etk_rect_h_rel_set(r, 50);
	etk_shape_color_set((Etk_Shape *)r, 0xaaaa0000);
	etk_shape_rop_set((Etk_Shape *)r, 0);
	ekeko_renderable_show((Ekeko_Renderable *)r);
#if 1
	/* let's animate something */
	a = etk_animation_new();
	ekeko_object_child_append((Ekeko_Object *)r, (Ekeko_Object *)a);
	etk_animation_property_set(a, "x");
	etk_coord_set(&coord, 0, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	etk_animation_from_set(a, &v);
	etk_coord_set(&coord, 100, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	etk_animation_to_set(a, &v);
	etk_value_clock_seconds_from(&v, &clock, 20);
	etk_animation_duration_set(a, &clock);
	etk_animation_calc_set(a, ETK_CALC_LINEAR);
	etk_animation_end_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_begin_set(a, (Ekeko_Object *)r, EVENT_UI_MOUSE_CLICK);
	etk_animation_repeat_set(a, -1);
#endif


	ekeko_object_dump((Ekeko_Object *)d, ekeko_object_dump_printf);
	etk_loop();
	etk_shutdown();

	return 0;
}

