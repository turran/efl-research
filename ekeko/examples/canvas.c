#include "Ekeko.h"
#include "ekeko_test.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Test_Canvas
{
	SDL_Surface *s;
} Test_Canvas;

static Eina_Bool _flush(Ekeko_Element *e, Eina_Rectangle *r)
{
	printf("canvas flushing!!!\n");
	//printf("RECT! %d %d %d %d\n", r->r.x, r->r.y, r->r.w, r->r.h);
	// FIXME for now, update the rect like this, maybe call the
	// UpdateRects directly? */
	//SDL_UpdateRect(c->surface, r->x, r->y, r->w, r->h);
	//SDL_Flip(c->surface);
	
	return EINA_FALSE;
}

static void _new(Ekeko_Element *e)
{
	SDL_Surface *s;
	Test_Canvas *tc;

	tc = malloc(sizeof(Test_Canvas));
	tc->s = NULL;
#if 0
	/* TODO check if the document already has a canvas element */
	SDL_Init(SDL_INIT_VIDEO);
	if (!(s = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return;
	}
#endif
	ekeko_canvas_new(e, _flush);
	ekeko_node_user_set((Ekeko_Node *)e, "test_canvas", tc);
}

static void _delete(Ekeko_Element *e)
{

}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void test_canvas_register(Ekeko_Document_Type *dt)
{
	Ekeko_Element_Type *et;
	//Ekeko_Value def;

	et = ekeko_document_type_element_register(dt, "test", "canvas",
		_new, _delete);
	//ekeko_value_int_from(&def, 100);
	//ekeko_document_type_element_attribute_register(et, "width", &def);
	//ekeko_document_type_element_attribute_register(et, "height", &def);
}
