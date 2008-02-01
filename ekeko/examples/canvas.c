#include "Ekeko.h"
#include "ekeko_test.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _flush(void *data, Ekeko_Rectangle *rects)
{
	Canvas *c = data;
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)rects; l; l = l->next)
	{
		Ekeko_Rectangle *r;
		r = (Ekeko_Rectangle *)l;
		//printf("RECT! %d %d %d %d\n", r->r.x, r->r.y, r->r.w, r->r.h);
		// FIXME for now, update the rect like this, maybe call the
		// UpdateRects directly? */
		SDL_UpdateRect(c->surface, r->r.x, r->r.y, r->r.w, r->r.h);
		//SDL_Flip(c->surface);
	}
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};


/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Canvas * canvas_new(int w, int h)
{
	Canvas *c;
	SDL_Surface *surface;
	
	
	SDL_Init(SDL_INIT_VIDEO);

	if (!(surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}
	
	c = calloc(1, sizeof(Canvas));
	//c->canvas = ekeko_canvas_new(&_canvas_class, c, EKEKO_TILER_TILEBUF, w, h);
	c->surface = surface;
	c->canvas = ekeko_canvas_new(&_canvas_class, c, EKEKO_TILER_SPLITTER, w, h);
	
	return c;
}

void canvas_process(Canvas *c)
{
	/* first process the list of subcanvas */
	/* then the canvas*/
	ekeko_canvas_process(c->canvas);
}
