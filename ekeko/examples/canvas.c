#include "Ekeko.h"
#include "ekeko_test.h"
#include <assert.h>
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Test_Canvas
{
	SDL_Surface *s;
} Test_Canvas;

#define TEST_CANVAS_PRIVATE "test_canvas"

static Eina_Bool _flush(Ekeko_Element *e, Eina_Rectangle *r)
{
	Test_Canvas *tc;
	printf("canvas flushing!!!\n");

	tc = ekeko_node_user_get((Ekeko_Node *)e, TEST_CANVAS_PRIVATE);
	//printf("RECT! %d %d %d %d\n", r->r.x, r->r.y, r->r.w, r->r.h);
	// FIXME for now, update the rect like this, maybe call the
	// UpdateRects directly? */
	SDL_UpdateRect(tc->s, r->x, r->y, r->w, r->h);
	SDL_Flip(tc->s);
	
	return EINA_FALSE;
}
static void _resize(Ekeko_Event *ee)
{
	Test_Canvas *tc;
	Ekeko_Event_Mutation *e = (Ekeko_Event_Mutation *)ee;
	Ekeko_Value v;
	
	if (strcmp(e->attr, CANVAS_GEOMETRY))
		return;
	tc = ekeko_node_user_get((Ekeko_Node *)e->related, TEST_CANVAS_PRIVATE);
	assert(tc);
	/* TODO check if the document already has a canvas element if so
	 * we should create a subcanvas :) */
	if (tc->s)
	{
		/* TODO free the surface */
	}
	ekeko_element_attribute_get((Ekeko_Element *)e->related, CANVAS_GEOMETRY, &v);
	tc->s = SDL_SetVideoMode(v.v.r.w, v.v.r.h, 32, SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF);
	if (!tc->s)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		abort();
		SDL_Quit();
		return;		
	}	
}
static void _new(Ekeko_Element *e)
{
	SDL_Surface *s;
	Test_Canvas *tc;

	tc = malloc(sizeof(Test_Canvas));
	tc->s = NULL;

	ekeko_canvas_new(e, _flush);
	ekeko_node_user_set((Ekeko_Node *)e, TEST_CANVAS_PRIVATE, tc);
	ekeko_node_event_listener_add(e, "DOMAttrUpdated",  _resize, EINA_FALSE);
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
