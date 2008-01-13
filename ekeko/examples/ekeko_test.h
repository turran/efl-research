#ifndef EKEKO_TEST_H_
#define EKEKO_TEST_H_

#include <SDL.h>

typedef struct _Subcanvas Subcanvas;
typedef struct _Canvas Canvas;
typedef struct _Object Object;

struct _Object
{
	Ekeko_Object *object;
	Canvas *canvas;
	void *data;
	int color;
};

struct _Canvas
{
	Ekeko_Canvas *canvas;
};


Canvas * canvas_new(void);

Subcanvas * subcanvas_new(Canvas *c);
Canvas * subcanvas_canvas_get(Subcanvas *s);
Object * subcanvas_object_get(Subcanvas *s);


Object * rectangle_new(Canvas *c);

Object * object_new(Canvas *c, Ekeko_Object_Class *class, void *data);
void object_move(Object *o, int x, int y);
void object_resize(Object *o, int w, int h);
void object_color_set(Object *o, int color);
Canvas * object_canvas_get(Object *o);

#endif /*EKEKO_TEST_H_*/
