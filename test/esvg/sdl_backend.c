#include <SDL.h>

SDL_Surface *surface;

int sdl_init(unsigned int w, unsigned int h)
{
	SDL_Init(SDL_INIT_VIDEO);

	if (!(surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	/* create a new software surface */
	/* create a new cairo context */
	return 1;
}

int sdl_loop(void)
{
	SDL_Event event;
	
	/* TODO blt the surface */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_ACTIVEEVENT:
			/*
			if (event.active.gain)
				ekeko_input_feed_mouse_in(input, 0);
			else
				ekeko_input_feed_mouse_out(input, 0);*/
			break;
		case SDL_MOUSEMOTION:
			//ekeko_input_feed_mouse_move(input, event.motion.x, event.motion.y, 0);        
			break;
		case SDL_MOUSEBUTTONDOWN:
			//ekeko_input_feed_mouse_move(input, event.button.x, event.button.y, 0);
			//ekeko_input_feed_mouse_down(input, event.button.state, EKEKO_BUTTON_NONE, 0);
			break;
		case SDL_MOUSEBUTTONUP:
			//ekeko_input_feed_mouse_move(input, event.button.x, event.button.y, 0);
			//ekeko_input_feed_mouse_up(input, event.button.state, EKEKO_BUTTON_NONE, 0);
			break;
		default:
			break;
			//case SDL_VIDEORESIZE:
		case SDL_QUIT:
			return 1;
			break;
		}
	}
	return 0;
}

void sdl_shutdown(void)
{

}
