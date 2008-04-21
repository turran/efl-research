#include <Etk.h>
#include <Etch.h>

Etk_Widget *window;
Etk_Widget *canvas;
Etk_Widget *etk_evas_object;
Evas_Object *evas_object;
Evas *evas;

Etch *etch;
Etch_Object *etch_object;


int timer_cb(void *data)
{
	etch_timer_tick(etch);
	return 1;
}

void _position_y_uint32(void *odata, void *pdata)
{
	int x, y;
	float val;
	
	val = *(unsigned int *)pdata / 10000.0;
	x = rint(200 + 200 * cos(val));
	y = rint(200 + 200 * sin(val));
	etk_canvas_move(ETK_CANVAS(canvas), etk_evas_object, x, y);
}

void _color(void *odata, void *pdata)
{
	unsigned int color = *(unsigned int *)pdata;
	unsigned char a, r, g, b;

	a = (color >> 24) & 0xff;
	r = (color >> 16) & 0xff;
	g = (color >> 8) & 0xff;
	b = color & 0xff;

	printf("called!!! 0x%x (%02x %02x %02x %02x)\n", color, a, r, g, b);
	evas_object_color_set(evas_object, r, g, b, a);
}

Etch_Object_Property oproperties[] = {
	{
		.type = ETCH_POSITION_Y_UINT32,
		.set = &_position_y_uint32,
	},
	{
		.type = ETCH_COLOR_ARGB,
		.set = &_color,
	},
};

Etch_Object_Class oclass = {
	.props = oproperties,
};


void etch_setup(void)
{
	Etch_Animation *ea;
	Etch_Animation_Keyframe *ek;

	etch = etch_new();
	etch_timer_fps_set(etch, 30);
	ecore_timer_add(1.0/30.0, timer_cb, NULL);
	
	etch_object = etch_object_add(etch, &oclass, "object1", NULL);
	/* position */
	ea = etch_animation_new(ETCH_UINT32);

	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 0);
	etch_animation_keyframe_time_set(ek, 0, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 2 * 31415);
	etch_animation_keyframe_time_set(ek, 20, 0);
	etch_object_animation_set(etch_object, ETCH_POSITION_Y, ea);

	/* color */
	ea = etch_animation_new(ETCH_ARGB);

	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 0xffff00ff);
	etch_animation_keyframe_time_set(ek, 0, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 0xff00ff00);
	etch_animation_keyframe_time_set(ek, 20, 0);
	etch_object_animation_set(etch_object, ETCH_COLOR, ea);
}

int main(int argc, char **argv)
{
	etk_init(argc, argv);

	window = etk_window_new();
	etk_window_title_set(ETK_WINDOW(window), "Etch-Etk Test");
	etk_window_resize(ETK_WINDOW(window), 640, 480);

	canvas = etk_canvas_new();
	etk_container_add(ETK_CONTAINER(window), canvas);

	evas = etk_toplevel_evas_get(ETK_TOPLEVEL(window));
	/* the circle boundaries
	evas_object = evas_object_rectangle_add(evas);
	evas_object_color_set(evas_object, 255, 255, 0, 255);
	evas_object_resize(evas_object, 100, 100);
	evas_object_move(evas_object, 100, 100);
	evas_object_show(evas_object);
	*/
	evas_object = evas_object_rectangle_add(evas);
	evas_object_color_set(evas_object, 255, 128, 128, 255);
	evas_object_resize(evas_object, 64, 64);
	

	etk_evas_object = etk_evas_object_new_from_object(evas_object);
	etk_canvas_put(ETK_CANVAS(canvas), etk_evas_object, 10, 10);

	etk_widget_show_all(window);

	etch_setup();
	etk_main();
	etk_shutdown();

	return 1;
}
