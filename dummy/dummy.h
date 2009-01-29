/*
 * dummy.h
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#ifndef DUMMY_H_
#define DUMMY_H_

#include "Etk2.h"
#include "Ecore.h"
#include "Ecore_Sdl.h"
#include "SDL.h"
#include "EXML.h"

typedef struct _Dummy_Rect_Private Dummy_Rect_Private;
typedef struct _Dummy_Rect
{
	Renderable parent;
	Dummy_Rect_Private *private;
} Dummy_Rect;

typedef struct _Dummy_Canvas_Private Dummy_Canvas_Private;
typedef struct _Dummy_Canvas
{
	Canvas parent;
	Dummy_Canvas_Private *private;
} Dummy_Canvas;

typedef struct _Dummy_Private Dummy_Private;
typedef struct _Dummy
{
	Object parent;
	Dummy_Private *private;
} Dummy;

/* types */
Type *dummy_type_get(void);
Type *dummy_canvas_type_get(void);
Type * dummy_rect_type_get(void);

/* dummy */
Dummy * dummy_new(void);
void dummy_loop(void);
Dummy_Canvas * dummy_canvas_get(Dummy *d);

/* dummy canvas */
Dummy_Canvas * dummy_canvas_new(void);
void dummy_canvas_resize(Dummy_Canvas *c, int w, int h);
SDL_Surface * dummy_canvas_surface_get(Dummy_Canvas *c);

/* dummy rect */
Dummy_Rect * dummy_rect_add(Dummy_Canvas *dc);
void dummy_rect_move(Dummy_Rect *r, int x, int y);
void dummy_rect_resize(Dummy_Rect *r, int w, int h);
void dummy_rect_color_set(Dummy_Rect *r, int color);

#endif /* DUMMY_H_ */
