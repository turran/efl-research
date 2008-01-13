#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Object * object_new(Canvas *c, Ekeko_Object_Class *class, void *data)
{
	Object *o;
	
	o = calloc(1, sizeof(Object));
	o->canvas = c;
	o->object = ekeko_object_add(c->canvas, class);
	o->data = data;
	ekeko_object_data_set(o->object, o);
	
	return o;
}

void object_move(Object *o, int x, int y)
{
	ekeko_object_move(o->object, x, y);
}

void object_resize(Object *o, int w, int h)
{
	
	ekeko_object_resize(o->object, w, h);
}

void object_color_set(Object *o, int color)
{
	o->color = color;
}

Canvas * object_canvas_get(Object *o)
{
	return o->canvas;
}
