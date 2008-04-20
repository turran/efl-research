#include <Etk.h>

Etk_Widget *window;
Etk_Widget *canvas;
Etk_Widget *etk_evas_object;
Evas_Object *evas_object;
Evas *evas;

int main(int argc, char **argv)
{
	etk_init(argc, argv);

	window = etk_window_new();
	etk_window_title_set(ETK_WINDOW(window), "Etch-Etk Test");
	etk_window_resize(ETK_WINDOW(window), 640, 480);

	canvas = etk_canvas_new();
	etk_container_add(ETK_CONTAINER(window), canvas);

	evas = etk_toplevel_evas_get(ETK_TOPLEVEL(window));
	evas_object = evas_object_rectangle_add(evas);
	evas_object_color_set(evas_object, 255, 128, 128, 255);
	evas_object_resize(evas_object, 64, 64);

	etk_evas_object = etk_evas_object_new_from_object(evas_object);
	etk_canvas_put(ETK_CANVAS(canvas), etk_evas_object, 10, 10);

	etk_widget_show_all(window);

	etk_main();
	etk_shutdown();

	return 1;
}
