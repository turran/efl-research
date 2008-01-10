#include "Ekeko.h"
#include <SDL.h>

#define CANVAS_W 640
#define CANVAS_H 480

#define RGBA(r, g, b, a)                                                \
    (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))

typedef struct _Rectangle
{
	Ekeko_Object *object;
} Rectangle;

static void sdl_rect_create(Ekeko_Object *o)
{
	Rectangle *rect;

	rect = malloc(sizeof(Rectangle));
	rect->object = o;
	ekeko_object_data_set(o, rect);
}

static void sdl_rect_free(void *data)
{
	free(data);
}

static void sdl_rect_pre_process(void *data)
{
	//printf("pre\n");
}

static void sdl_rect_process(void *data, Enesim_Rectangle *r)
{
	Rectangle *sdl_rect;
	Ekeko_Canvas *canvas;
	SDL_Rect rect;
	SDL_Surface *surface;

	sdl_rect = data;
	canvas = ekeko_object_canvas_get(sdl_rect->object);
	surface = ekeko_canvas_data_get(canvas);
	rect.x = 10;
	rect.y = 10;
	rect.w = 50;
	rect.h = 50;
	SDL_FillRect(surface, &rect, RGBA(255, 255, 255, 255));
}

static void sdl_rect_post_process(void *data)
{

}

Ekeko_Object_Class sdl_rectangle = {
	.create = sdl_rect_create,
	.free = sdl_rect_free,
	.pre_process = sdl_rect_pre_process,
	.process = sdl_rect_process,
	.post_process = sdl_rect_post_process,
};

int main(int argc, char **argv)
{
	Ekeko_Canvas *c;
	Ekeko_Object *o;
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
	o = ekeko_object_add(c, &sdl_rectangle);
	ekeko_object_move(o, 10, 10);
	ekeko_object_resize(o, 50, 50);
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
		ekeko_canvas_process(c);
	}
	SDL_Quit();
}
