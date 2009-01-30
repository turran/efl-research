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
#if 0
		Ekeko_Value v;
		
		ekeko_value_bool_from(&v, EINA_TRUE);
		ekeko_node_attribute_set(e->related, RENDERABLE_OPAQUE, &v);
#endif
	}
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void test_object_new(Ekeko_Element *e)
{
	ekeko_element_attribute_int_set(e, OBJECT_COLOR, 0x0);
	ekeko_node_event_listener_add((Ekeko_Node *)e, "DOMAttrModified",  _attr_modified_cb,
		EINA_FALSE);
}
#if 0
Ekeko_Object * ekeko_object_new(Ekeko_Canvas *c, Ekeko_Object_Class *class, void *cdata)
{
	Ekeko_Object *o;
	
	o = calloc(1, sizeof(Ekeko_Object));
	o->canvas = c;
	/* if cdata then pass it, else use the default */
	if (cdata)
		o->object = ekeko_object_add(c->canvas, class, cdata);
	else
		o->object = ekeko_object_add(c->canvas, class, o);
	
	return o;
}

void object_move(Ekeko_Object *o, int x, int y)
{
	ekeko_object_move(o->object, x, y);
}

void object_resize(Ekeko_Object *o, int w, int h)
{
	
	ekeko_object_resize(o->object, w, h);
}

void object_color_set(Ekeko_Object *o, int color)
{
	o->color = color;
}

Ekeko_Canvas * object_canvas_get(Ekeko_Object *o)
{
	return o->canvas;
}
#endif
