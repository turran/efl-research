#ifndef _BUILDER_H
#define _BUILDER_H

#include <Eon.h>

typedef struct _Shape Shape;
typedef struct _Control Control;

enum Shape_State
{
	NONE,
	SCALE,
	ROTATE,
	PROJECT,
};

typedef enum Control_Poisitions
{
	TOP_LEFT,
	TOP_MIDDLE,
	TOP_RIGHT,
	CENTER_LEFT,
	CENTER_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_MIDDLE,
	BOTTOM_RIGHT,
	CONTROL_POSITIONS,
} Control_Position;

typedef void (*Control_Move)(Control *, int dx, int dy);
typedef void (*Shape_Move)(Shape *, int dx, int dy);

struct _Shape
{
	Eon_Shape *sh;
	Eon_Shape *area;
	Eon_Shape *cp[CONTROL_POSITIONS];
	enum Shape_State state;
	Eina_Bool transforming;
	/* callbacks */
	Control_Move cmove;
	Shape_Move smove;
};

struct _Control
{
	Shape *ref;
	Eon_Shape *sh;
	Control_Position pos;
};

typedef struct _Scene
{
	Eon_Shape *sh;
	Eina_List *selected;
} Scene;

Scene * scene_new(Eon_Canvas *canvas);

Shape * shape_new(Eon_Shape *es, Control_Move cmove, Shape_Move smove);

Control *control_new(Shape *ref, Control_Position pos);
void control_show(Control *cp);
void control_hide(Control *cp);
void control_update(Control *cp);

#endif
