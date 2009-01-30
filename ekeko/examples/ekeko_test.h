#ifndef EKEKO_TEST_H_
#define EKEKO_TEST_H_

#include <SDL.h>

#define OBJECT_COLOR "color"

#define RGBA(r, g, b, a)                                                \
    (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))

SDL_Surface * test_canvas_surface_get(Ekeko_Element *e);
void test_object_new(Ekeko_Element *e);

#if 0
typedef struct _Subcanvas Subcanvas;
typedef struct _Canvas Ekeko_Canvas;
typedef struct _Ekeko_Object Ekeko_Object;

struct _Ekeko_Object
{
	Ekeko_Object *object;
	Ekeko_Canvas *canvas;
	int color;
	void *data;
};

struct _Canvas
{
	Ekeko_Canvas *canvas;
	SDL_Surface *surface;
	/* TODO list of subcanvas */
};

Ekeko_Canvas * ekeko_canvas_new(int w, int h);
void canvas_process(Ekeko_Canvas *c);

Subcanvas * subcanvas_new(Ekeko_Canvas *c, int x, int y, int w, int h);
Ekeko_Canvas * subcanvas_canvas_get(Subcanvas *s);
Ekeko_Object * subcanvas_object_get(Subcanvas *s);


Ekeko_Object * rectangle_new(Ekeko_Canvas *c);
Ekeko_Object * filter_new(Ekeko_Canvas *c);
Ekeko_Object * ekeko_object_new(Ekeko_Canvas *c, Ekeko_Object_Class *class, void *cdata);
void object_move(Ekeko_Object *o, int x, int y);
void object_resize(Ekeko_Object *o, int w, int h);
void object_color_set(Ekeko_Object *o, int color);
Ekeko_Canvas * object_canvas_get(Ekeko_Object *o);


static inline void rectangle_print(Eina_Rectangle *r)
{
	printf("%d %d %d %d\n", r->x, r->y, r->w, r->h);
}
#endif
#endif /*EKEKO_TEST_H_*/
