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

void _position_x_uint32(void *odata, void *pdata)
{
	int x, y;

	x = *(unsigned int *)pdata;
	printf("called with x %d\n", x);
	evas_object_geometry_get(evas_object, NULL, &y, NULL, NULL);
	evas_object_move(evas_object, x, y);
}

void _position_y_uint32(void *odata, void *pdata)
{
	int x, y;
	float val;
#if 0
	int x, y;

	y = *(unsigned int *)pdata;
	printf("called with y %d\n", y);
	evas_object_geometry_get(evas_object, &x, NULL, NULL, NULL);
	evas_object_move(evas_object, x, y);
#endif
	val = *(unsigned int *)pdata / 10000.0;
	x = 200 + 200 * cos(val);
	y = 200 + 200 * sin(val);
	evas_object_move(evas_object, x, y);
}

Etch_Object_Property oproperties[] = {
	{
		.type = ETCH_POSITION_X_UINT32,
		.set = &_position_x_uint32, 
	},
	{
		.type = ETCH_POSITION_Y_UINT32,
		.set = &_position_y_uint32,
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
#if 0
	/* x coordinate */
	ea = etch_animation_new(ETCH_UINT32);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 200, 200);
	etch_animation_keyframe_time_set(ek, 0, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 300, 300);
	etch_animation_keyframe_time_set(ek, 5, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 200, 200);
	etch_animation_keyframe_time_set(ek, 10, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 100, 100);
	etch_animation_keyframe_time_set(ek, 15, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 200, 200);
	etch_animation_keyframe_time_set(ek, 20, 0);
	/* append the animation to the object */
	etch_object_animation_set(etch_object, ETCH_POSITION_X_UINT32, ea);
	
	/* y coordinate */
	ea = etch_animation_new(ETCH_UINT32);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 200, 200);
	etch_animation_keyframe_time_set(ek, 0, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 100, 100);
	etch_animation_keyframe_time_set(ek, 5, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 0, 0);
	etch_animation_keyframe_time_set(ek, 10, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 100, 100);
	etch_animation_keyframe_time_set(ek, 15, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_QUADRATIC);
	etch_animation_keyframe_value_set(ek, 200, 200);
	etch_animation_keyframe_time_set(ek, 20, 0);
		
	/* append the animation to the object */
	etch_object_animation_set(etch_object, ETCH_POSITION_Y_UINT32, ea);
#endif
	ea = etch_animation_new(ETCH_UINT32);

	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 0);
	etch_animation_keyframe_time_set(ek, 0, 0);
	
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 31415);
	etch_animation_keyframe_time_set(ek, 20000, 0);
	etch_object_animation_set(etch_object, ETCH_POSITION_Y_UINT32, ea);
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
