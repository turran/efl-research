#include "Ekeko.h"
#include "ekeko_test.h"


Canvas *c;
Subcanvas *subcanvas;
Object *background1;
Object *background2;
Object *rectangle1;
Object *rectangle2;

/**
 * TODO
 * clean up the sdl_rectangle[class] code, abstract correctly the ekeko_object (WIP)
 * make this app test different things evas can't handle, like new object types (filters, subcanvas, mirror objects, etc)
 * add support for benchmarking different tilers (rect_splitter, tilebuf, etc)
 * 
 */

#define CANVAS_W 640
#define CANVAS_H 480


void init(void)
{
	c = canvas_new(CANVAS_W, CANVAS_H);
	/* background */
	background1 = rectangle_new(c);
	object_move(background1, 0, 0);
	object_resize(background1, CANVAS_W, CANVAS_H);
	object_color_set(background1, RGBA(255, 255, 255, 255));
	/* object moving */
	rectangle1 = rectangle_new(c);
	object_move(rectangle1, 0, 0);
	object_resize(rectangle1, 50, 50);
	object_color_set(rectangle1, RGBA(255, 0, 0, 255));
	/* subcanvas */
#if 1
	subcanvas = subcanvas_new(c, 319, 239, 320, 240);
	background2 = rectangle_new(subcanvas_canvas_get(subcanvas));
	object_move(background2, 0, 0);
	object_resize(background2, 320, 240);
	object_color_set(background2, RGBA(0, 255, 255, 255));
#endif
}

void shutdown(void)
{
	//subcanvas_delete(s);
	//object_delete(background);
	//object_delete(rectangle);
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
		/* TODO this should inside canvas process, do we actually need to call it? */
		canvas_process(subcanvas_canvas_get(subcanvas));
		i++;
		if (i > 1000)
		{
			r.x = (r.x + 1) % CANVAS_W;
			if (r.x == 0)
			{
				r.y = (r.y + 1) % CANVAS_H;
			}
			object_move(rectangle1, r.x, r.y);
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
