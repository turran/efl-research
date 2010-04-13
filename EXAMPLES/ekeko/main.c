#include "Ekeko.h"

static void _canvas_new(Ekeko_Element *e)
{
	printf("canvas new\n");
	//ekeko_canvas_new(e);
}

static void _canvas_delete(Ekeko_Element *e)
{

}

static void _rect_new(Ekeko_Element *e)
{
	printf("rectangle new\n");
}

static void _rect_delete(Ekeko_Element *e)
{

}

void init_register(void)
{
	Ekeko_Document_Type *dt;

	dt = ekeko_document_register("test");
	ekeko_document_element_register(dt, "test", "canvas", _canvas_new,
		_canvas_delete);
	ekeko_document_element_register(dt, "test", "rect", _rect_new,
		_rect_delete);
}

void init_scene(void)
{
	Ekeko_Document *d;
	Ekeko_Element *canvas, *obj;

	d = ekeko_document_new("test", "1");
	canvas = ekeko_document_element_new(d, "test", "canvas");
	obj = ekeko_document_element_new(d, "test", "rect");
}

void main_loop(void)
{
	for(;;)
	{

	}
}

int main(void)
{
	ekeko_init();
	init_register();
	init_scene();
	main_loop();
	ekeko_shutdown();
	return 0;
}
