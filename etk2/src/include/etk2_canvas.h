/*
 * etk2_canvas.h
 *
 *  Created on: 12-ene-2009
 *      Author: jl
 */
#ifndef ETK2_CANVAS_H_
#define ETK2_CANVAS_H_

typedef struct _Canvas_Private Canvas_Private;

struct _Canvas
{
	Object parent;
	Canvas_Private *private;
};

Type *canvas_type_get(void);

#endif /* ETK2_CANVAS_H_ */
