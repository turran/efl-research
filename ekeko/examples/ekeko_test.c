#include "Ekeko.h"
#include <SDL.h>

#define CANVAS_W 640
#define CANVAS_H 480


int main(int argc, char **argv)
{
	Ekeko_Canvas *c;
	SDL_Event event;
	SDL_Surface *surface;
	int end = 0;
	
	SDL_Init(SDL_INIT_VIDEO);
	
	if (!(surface = SDL_SetVideoMode(CANVAS_W, CANVAS_H, 32, SDL_RESIZABLE)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	c = ekeko_canvas_new(EKEKO_TILER_SPLITTER, CANVAS_W, CANVAS_H);
	ekeko_canvas_data_set(c, surface);
	while (!end)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				default:
				break;
				//case SDL_VIDEORESIZE:

				case SDL_QUIT:
				end = 1;
				break;
			}
		}
	}
	SDL_Quit();
}
