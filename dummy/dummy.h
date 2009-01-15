/*
 * dummy.h
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#ifndef DUMMY_H_
#define DUMMY_H_

#include "Etk2.h"
//#include "SDL.h"

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
void dummy_loop(Dummy *d);
Dummy_Canvas * dummy_canvas_get(Dummy *d);

/* dummy canvas */

/* dummy rect */
Dummy_Rect * dummy_rect_add(Dummy_Canvas *dc);

#endif /* DUMMY_H_ */
