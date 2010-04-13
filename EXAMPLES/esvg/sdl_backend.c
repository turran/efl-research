#include "esvg_test.h"

SDL_Surface *screen;
SDL_Surface *src;
cairo_surface_t *src_cairo;

int sdl_init(unsigned int w, unsigned int h)
{
	SDL_Init(SDL_INIT_VIDEO);
	unsigned char *src_data;
	unsigned int stride;

	if (!(screen = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SWSURFACE)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	/* create a new software surface */
	stride = w * 4;
	src_data = calloc(stride * h, sizeof(unsigned char ));
	src = SDL_CreateRGBSurfaceFrom((void *)src_data, w, h, 32, stride,
	0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	/* create a new cairo context */
	src_cairo = cairo_image_surface_create_for_data(src_data, CAIRO_FORMAT_ARGB32, w, h, stride);

	return 1;
}

int sdl_loop(ESVG_Svg *svg)
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
		case SDL_VIDEORESIZE:
		{
			ESVG_Engine *engine = esvg_svg_engine_get(svg);
			esvg_engine_output_size_set(engine, event.resize.w, event.resize.h);

			break;
		}
		case SDL_QUIT:
			return 1;
			break;
		}
	}
	SDL_BlitSurface(src, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, 0, 0);

	return 0;
}

void sdl_shutdown(void)
{

}
