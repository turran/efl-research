#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Object * object_new(Canvas *c, Ekeko_Object_Class *class, void *cdata)
{
	Object *o;
	
	o = calloc(1, sizeof(Object));
	o->canvas = c;
	/* if cdata then pass it, else use the default */
	if (cdata)
		o->object = ekeko_object_add(c->canvas, class, cdata);
	else
		o->object = ekeko_object_add(c->canvas, class, o);
	
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
