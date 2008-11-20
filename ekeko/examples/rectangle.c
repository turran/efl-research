#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _render(void *data, Eina_Rectangle *r)
{
	SDL_Rect rect;
	
	//printf("(%d) %d %d %d %d\n", o->color, r->x, r->y, r->w, r->h);
	rect.x = r->x;
	rect.y = r->y;
	rect.w = r->w;
	rect.h = r->h;
	//SDL_FillRect(o->canvas->surface, &rect, o->color);
}
static void _new(Ekeko_Element *e)
{
	Ekeko_Renderable_Class rclass;

	rclass.process = _render;
	ekeko_renderable_new(e, &rclass);
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
	Ekeko_Value def;

	et = ekeko_document_type_element_register(dt, "test", "rect", _new,
		_delete);
	ekeko_value_rectangle_coords_from(&def, 100);
	ekeko_document_type_element_attribute_register(et, "geom", &def);
}
