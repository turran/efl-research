#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Canvas * canvas_new(int w, int h)
{
	Canvas *c;
	SDL_Surface *surface;
	
	
	SDL_Init(SDL_INIT_VIDEO);

	if (!(surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}
	
	c = calloc(1, sizeof(Canvas));
	c->canvas = ekeko_canvas_new(EKEKO_TILER_TILEBUF, w, h);
	//c->canvas = ekeko_canvas_new(EKEKO_TILER_SPLITTER, w, h);
	ekeko_canvas_data_set(c->canvas, surface);
	
	return c;
}

void canvas_process(Canvas *c)
{
	ekeko_canvas_process(c->canvas);
}
