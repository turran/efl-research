#include "Ekeko.h"
#include "ekeko_test.h"


Canvas *c;
Object *rectangle;

/**
 * TODO
 * clean up the sdl_rectangle[class] code, abstract correctly the ekeko_object (WIP)
 * make this app test different things evas can't handle, like new object types (filters, subcanvas, mirror objects, etc)
 * add support for benchmarking different tilers (rect_splitter, tilebuf, etc)
 * 
 */

#define CANVAS_W 640
#define CANVAS_H 480

#define RGBA(r, g, b, a)                                                \
    (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))


void init(void)
{
	c = canvas_new(CANVAS_W, CANVAS_H);
	/* background */
	rectangle = rectangle_new(c);
	object_move(rectangle, 0, 0);
	object_resize(rectangle, CANVAS_W, CANVAS_H);
	object_color_set(rectangle, RGBA(255, 255, 255, 255));
	
	/* object moving */
	rectangle = rectangle_new(c);
	object_move(rectangle, 0, 0);
	object_resize(rectangle, 50, 50);
	object_color_set(rectangle, RGBA(255, 255, 0, 255));

}

void shutdown(void)
{
	
}


void loop(void)
{
	Enesim_Rectangle r;
	SDL_Event event;
	int end = 0;
	int i = 0;
	
	r.x = 0;
	r.y = 0;
	r.w = 50;
	r.h = 50;
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
		canvas_process(c);
		i++;
		if (i > 1000)
		{
			r.x = (r.x + 1) % CANVAS_W;
			if (r.x == 0)
			{
				r.y = (r.y + 1) % CANVAS_H;
			}
			object_move(rectangle, r.x, r.y);
			i = 0;
		}
	}
	SDL_Quit();
}


int main(int argc, char **argv)
{
	init();
	loop();
	return 0;
}
