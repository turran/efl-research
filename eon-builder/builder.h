#ifndef _BUILDER_H
#define _BUILDER_H

#include <Eon.h>
#include <stdio.h>
#include <math.h>

typedef struct _Scene Scene;
typedef struct _Shape Shape;
typedef struct _Control Control;

typedef enum Shape_State
{
	NONE,
	SCALE,
	ROTATE,
	PROJECT,
	SHAPE_STATES,
} Shape_State;

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
	Scene *sc;
	float angle;
};

struct _Control
{
	Shape *ref;
	Eon_Shape *sh;
	Control_Position pos;
};

struct _Scene
{
	Eon_Canvas *canvas;
	Eon_Shape *sh;
	Eina_List *selected;
};

Scene * scene_new(Eon_Canvas *canvas);
void scene_selected_remove(Scene *s, Shape *sh);
void scene_selected_add(Scene *s, Shape *sh);

Shape * shape_new(Eon_Shape *es, Scene *sc, Control_Move cmove,
		Shape_Move smove);
void shape_state_set(Shape *s, Shape_State st);

Control * control_new(Shape *ref, Control_Position pos);
void control_show(Control *cp);
void control_hide(Control *cp);
void control_update(Control *cp);

Shape * circle_new(Scene *sc);
Shape * rect_new(Scene *sc);

#endif
