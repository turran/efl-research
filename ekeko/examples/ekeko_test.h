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
typedef struct _Canvas Canvas;
typedef struct _Object Object;

struct _Object
{
	Ekeko_Object *object;
	Canvas *canvas;
	int color;
	void *data;
};

struct _Canvas
{
	Ekeko_Canvas *canvas;
	SDL_Surface *surface;
	/* TODO list of subcanvas */
};

Canvas * canvas_new(int w, int h);
void canvas_process(Canvas *c);

Subcanvas * subcanvas_new(Canvas *c, int x, int y, int w, int h);
Canvas * subcanvas_canvas_get(Subcanvas *s);
Object * subcanvas_object_get(Subcanvas *s);


Object * rectangle_new(Canvas *c);
Object * filter_new(Canvas *c);
Object * object_new(Canvas *c, Ekeko_Object_Class *class, void *cdata);
void object_move(Object *o, int x, int y);
void object_resize(Object *o, int w, int h);
void object_color_set(Object *o, int color);
Canvas * object_canvas_get(Object *o);


static inline void rectangle_print(Eina_Rectangle *r)
{
	printf("%d %d %d %d\n", r->x, r->y, r->w, r->h);
}
#endif
#endif /*EKEKO_TEST_H_*/
