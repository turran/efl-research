#include "Ekeko.h"
#include "ekeko_test.h"

/**
 * TODO
 * clean up the sdl_rectangle[class] code, abstract correctly the ekeko_object
 */

#define CANVAS_W 640
#define CANVAS_H 480

#define RGBA(r, g, b, a)                                                \
    (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))


void init(void)
{
	Canvas *c;
	
	c = canvas_new();
	/* background */
#if 0
	o = ekeko_object_add(c, &sdl_rectangle_class);
	enesim_rectangle_coords_from(&rect, 0, 0, CANVAS_W, CANVAS_H);
	ekeko_object_move(o, rect.x, rect.y);
	ekeko_object_resize(o, rect.w, rect.h);
	sdl_rect = ekeko_object_data_get(o);
	sdl_rect->color = RGBA(255, 255, 255, 255);
	printf("background = %d\n", sdl_rect->color);
	/* object moving */
	o = ekeko_object_add(c, &sdl_rectangle_class);
	enesim_rectangle_coords_from(&rect, 10, 10, 50, 50);
	ekeko_object_move(o, rect.x, rect.y);
	ekeko_object_resize(o, rect.w, rect.h);
	sdl_rect = ekeko_object_data_get(o);
	sdl_rect->color = RGBA(255, 0, 0, 255);
	printf("object = %d\n", sdl_rect->color);
#endif
}

void shutdown(void)
{
	
}


void loop(void)
{
	SDL_Event event;
	int end = 0;
	int i = 0;
	
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
#if 0
		ekeko_canvas_process(c);
		
		i++;
		if (i > 10000)
		{
			rect.x = (rect.x + 1) % CANVAS_W;
			if (rect.x == 0)
			{
				rect.y = (rect.y + 1) % CANVAS_H;
			}
			ekeko_object_move(o, rect.x, rect.y);
			i = 0;
		}
#endif
	}
	SDL_Quit();
}


int main(int argc, char **argv)
{
	init();
	loop();
	return 0;
}
