#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Canvas * canvas_new(void)
{
	Canvas *c;
#if 0
	Ekeko_Canvas *c;
	Ekeko_Object *o;
	Enesim_Rectangle rect;
	Rectangle *sdl_rect;
	
	SDL_Surface *surface;
	

	SDL_Init(SDL_INIT_VIDEO);

	if (!(surface = SDL_SetVideoMode(CANVAS_W, CANVAS_H, 32, SDL_RESIZABLE)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//c = ekeko_canvas_new(EKEKO_TILER_SPLITTER, CANVAS_W, CANVAS_H);
	c = ekeko_canvas_new(EKEKO_TILER_TILEBUF, CANVAS_W, CANVAS_H);
	ekeko_canvas_data_set(c, surface);
#endif
	return c;
}
