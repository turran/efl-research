#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/* TODO when the color has changed, modify the opaqueness */
static void _attr_modified_cb(Ekeko_Event *ee)
{
	Ekeko_Event_Mutation *e = (Ekeko_Event_Mutation *)ee;
		
	if (!strcmp(e->attr, OBJECT_COLOR))
	{
		Ekeko_Value v;
		
		ekeko_value_bool_from(&v, EINA_TRUE);
		ekeko_node_attribute_set(e->related, RENDERABLE_OPAQUE, &v);
	}
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void test_object_new(Ekeko_Element *e)
{
	Ekeko_Value def;

	ekeko_value_int_from(&def, 0x0);
	ekeko_element_attribute_set(e, OBJECT_COLOR, &def);
	ekeko_node_event_listener_add((Ekeko_Node *)e, "DOMAttrModified",  _attr_modified_cb,
		EINA_FALSE);
}
#if 0
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
#endif
