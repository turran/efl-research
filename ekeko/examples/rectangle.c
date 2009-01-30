#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _render(Ekeko_Element *c, Ekeko_Element *e, Eina_Rectangle *r)
{
	SDL_Rect rect;
	SDL_Surface *s;
	Ekeko_Value *v;
	
	s = test_canvas_surface_get(c);
	v = ekeko_element_attribute_get(e, OBJECT_COLOR);
	rect.x = r->x;
	rect.y = r->y;
	rect.w = r->w;
	rect.h = r->h;
	SDL_FillRect(s, &rect, ekeko_value_int_get(v));
}
static void _new(Ekeko_Element *e)
{
	Ekeko_Renderable_Class rclass;

	rclass.render = _render;
	ekeko_renderable_new(e, &rclass);
	test_object_new(e);
}

static void _delete(Ekeko_Element *e)
{

}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void test_rect_register(Ekeko_Document_Type *dt)
{
	Ekeko_Element_Type *et;

	et = ekeko_document_type_element_register(dt, "test", "rect", _new,
		_delete);
}
