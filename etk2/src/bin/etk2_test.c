#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <Etk2.h>

static void _color_animation(Ekeko_Object *o, const char *prop,
		Etk_Color orig, Etk_Color dst, unsigned int secs)
{
	Etk_Animation *a;
	Etk_Clock clock;
	Ekeko_Value v;

	a = etk_animation_new();
	ekeko_object_child_append((Ekeko_Object *)o, (Ekeko_Object *)a);
	etk_animation_property_set(a, prop);
	etk_animation_begin_set(a, (Ekeko_Object *)o, EVENT_UI_MOUSE_CLICK);
	etk_value_color_from(&v, orig);
	etk_animation_from_set(a, &v);
	etk_value_color_from(&v, dst);
	etk_animation_to_set(a, &v);
	etk_value_clock_seconds_from(&v, &clock, secs);
	etk_animation_duration_set(a, &clock);
	etk_animation_calc_set(a, ETK_CALC_LINEAR);
	etk_animation_end_set(a, (Ekeko_Object *)o, EVENT_UI_MOUSE_CLICK);
	etk_animation_repeat_set(a, -1);
}

static void _coord_animation(Ekeko_Object *o, const char *prop,
		int orig, Etk_Coord_Type torig, int dst, Etk_Coord_Type tdst,
		unsigned int secs)
{
	Etk_Animation *a;
	Etk_Coord coord;
	Etk_Clock clock;
	Ekeko_Value v;

	a = etk_animation_new();
	ekeko_object_child_append((Ekeko_Object *)o, (Ekeko_Object *)a);
	etk_animation_property_set(a, prop);
	etk_coord_set(&coord, orig, torig);
	etk_value_coord_from(&v, &coord);
	etk_animation_from_set(a, &v);
	etk_coord_set(&coord, dst, tdst);
	etk_value_coord_from(&v, &coord);
	etk_animation_to_set(a, &v);
	etk_value_clock_seconds_from(&v, &clock, secs);
	etk_animation_duration_set(a, &clock);
	etk_animation_calc_set(a, ETK_CALC_LINEAR);
	etk_animation_end_set(a, (Ekeko_Object *)o, EVENT_UI_MOUSE_CLICK);
	etk_animation_begin_set(a, (Ekeko_Object *)o, EVENT_UI_MOUSE_CLICK);
	etk_animation_repeat_set(a, -1);
}

static Etk_Filter * _filter_add(Etk_Canvas *c)
{
	Etk_Filter *f;
	Etk_Fe_Displace *disp;

	f = etk_filter_new(c);
	//disp = etk2_fe_displace_new(f);
	return f;
}

static void _filter_apply(Etk_Shape *s, Etk_Filter *f)
{
	printf("Applying filter!!!\n");
	etk_shape_filter_set(s, f);
}

static void _click_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Enesim_Matrix m;
	static Etk_Filter *f1 = NULL;
	static Etk_Filter *f2 = NULL;
	static Etk_Filter *curr = NULL;

#if 0
	Eina_Rectangle r;
	printf("clicked!!\n");

	eina_rectangle_coords_from(&r, 200, 200, 100, 400);
	ekeko_canvas_damage_add((Ekeko_Canvas *)data, &r);
#endif
#if 0
	enesim_matrix_identity(&m);
	enesim_matrix_scale(&m, 1, 1);
	etk_image_matrix_set((Etk_Image *)data, &m);
#endif
	if (!f1)
	{
		Etk_Canvas *c;

		c = etk_shape_canvas_get((Etk_Shape *)data);
		f1 = _filter_add(c);
		f2 = _filter_add(c);
		curr = f1;
	}
	if (curr == f1)
	{

		curr = f2;
	}
	else
	{
		curr = f1;
	}
	_filter_apply((Etk_Shape *)data, curr);
}

static void _setup_scene(Etk_Canvas *c)
{
	Etk_Image *i;
	Etk_Rect *r;

#if 1
	/* create an image */
	i = etk_image_new(c);
	//etk_image_x_set(i, 30);
	//etk_image_y_set(i, 30);
	//etk_image_w_set(i, 400);
	//etk_image_h_set(i, 412);
	//etk_image_file_set(i, "/home/jl/code/efl-research/trunk/etk2/data/tiger.png");
	etk_image_file_set(i, "/home/jl/checker.png");
	etk_image_x_set(i, 150);
	etk_image_y_set(i, 150);
	etk_image_w_set(i, 256);
	etk_image_h_set(i, 256);
	etk_image_show(i);
#endif
#if 1
	/* create a rectangle */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 25);
	etk_rect_y_rel_set(r, 25);
	etk_rect_w_rel_set(r, 50);
	etk_rect_h_rel_set(r, 50);
	etk_rect_color_set(r, 0xaaaa0000);
	etk_rect_rop_set(r, ENESIM_BLEND);
	etk_rect_show(r);
#endif
	_color_animation((Ekeko_Object *)r, "color", 0xff00ff00, 0xaaffaaff, 30);
	//_coord_animation((Ekeko_Object *)r, "w", 10, ETK_COORD_RELATIVE, 100, ETK_COORD_RELATIVE, 30);
	//ekeko_event_listener_add((Ekeko_Object *)i, EVENT_UI_MOUSE_DOWN, _click_cb, EINA_FALSE, c);
	//ekeko_event_listener_add((Ekeko_Object *)r, EVENT_UI_MOUSE_DOWN, _click_cb, EINA_FALSE, r);

#if 0
	r = etk_rect_new(sc);
	etk_rect_x_rel_set(r, 0);
	etk_rect_y_rel_set(r, 0);
	etk_rect_w_rel_set(r, 100);
	etk_rect_h_rel_set(r, 100);
	etk_rect_color_set(r, 0xaaaa00aa);
	etk_rect_rop_set(r, ENESIM_FILL);
	etk_rect_show(r);
#endif
}

int main(int argc, char **argv)
{
	Etk_Document *d;
	Etk_Canvas *c, *sc;
	Etk_Rect *r;
	Enesim_Matrix mx1, mx2;

	etk_init();

	d = etk_document_new("sdl", 480, 640);
	c = etk_document_canvas_get(d);
	etk_canvas_w_rel_set(c, 100);
	etk_canvas_h_rel_set(c, 100);

	/* create the background */
	r = etk_rect_new(c);
	etk_rect_x_rel_set(r, 0);
	etk_rect_y_rel_set(r, 0);
	etk_rect_w_rel_set(r, 100);
	etk_rect_h_rel_set(r, 100);
	etk_rect_color_set(r, 0xffffffff);
	etk_rect_rop_set(r, ENESIM_FILL);
	etk_rect_show(r);

	_setup_scene(c);
#if 0
	/* add a subcanvas */
	sc = etk_canvas_new(c);
	etk_canvas_x_rel_set(sc, 0);
	etk_canvas_y_rel_set(sc, 0);
	etk_canvas_w_rel_set(sc, 50);
	etk_canvas_h_rel_set(sc, 50);

	enesim_matrix_scale(&mx1, 0.8, 0.8);
	/*
	enesim_matrix_rotate(&mx1, 1.3);
	enesim_matrix_translate(&mx2, 60, 20);
	enesim_matrix_compose(&mx2, &mx1, &mx1);
	enesim_matrix_translate(&mx2, -60, -20);
	enesim_matrix_compose(&mx1, &mx2, &mx1);
	//enesim_matrix_translate(&mx1, 30, 30);

	*/
	etk_canvas_matrix_set(sc, &mx1);
	_setup_scene(sc);
	etk_canvas_show(sc);
#endif
	ekeko_object_dump((Ekeko_Object *)d, ekeko_object_dump_printf);
	etk_loop();
	etk_shutdown();

	return 0;
}

