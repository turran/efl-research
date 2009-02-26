/*
 * etk_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef _ETK2_PRIVATE_H
#define _ETK2_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
//#define ETK_DEBUG

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

/* Type names */
#define ETK_TYPE_DOCUMENT "Etk_Document"
#define ETK_TYPE_CANVAS "Etk_Canvas"
#define ETK_TYPE_RECT "Etk_Rect"
#define ETK_TYPE_IMAGE "Etk_Image"
#define ETK_TYPE_ANIMATION "Etk_Animation"

/* Engine! */
typedef void * Etk_Engine_Surface;

typedef struct _Etk_Document_Engine
{
	void * (*create)(Etk_Document *);
	void (*delete)(Etk_Document *, void *);
} Etk_Document_Engine;

typedef struct _Etk_Canvas_Engine
{
	void (*lock)(void *s);
	void (*unlock)(void *s);
	void * (*create)(Eina_Bool root, int w, int h);
	void (*blit)(void *src, Eina_Rectangle *srect, void *context, void *dst, Eina_Rectangle *drect);
	Eina_Bool (*flush)(void *src, Eina_Rectangle *srect);
} Etk_Canvas_Engine;

typedef struct _Etk_Context_Engine
{
	void * (*create)(void);
	void (*delete)(void *context);
	void (*color_set)(void *context, int color); /* FIXME change this int */
	void (*rop_set)(void *context, int rop); /* FIXME change this int */
	void (*matrix_set)(void *c, Enesim_Matrix *m);

} Etk_Context_Engine;

typedef struct _Etk_Shape_Engine
{
	void (*rect)(void *surface, void *context, int x, int y, int w, int h);
} Etk_Shape_Engine;

struct _Etk_Engine
{
	Etk_Document_Engine *doc;
	Etk_Canvas_Engine *canvas;
	Etk_Context_Engine *context;
	Etk_Shape_Engine *shape;
};

void etk_engine_init(void);
void etk_engine_shutdown(void);
void etk_engine_register(Etk_Engine *engine, const char *name);
Etk_Engine * etk_engine_get(const char *name);

/* Enesim engine */
extern Etk_Context_Engine etk_context_engine_enesim;
extern Etk_Shape_Engine etk_shape_engine_enesim;

/* SDL engine */
void engine_sdl_init(void);

/* Ekeko_Value */
void etk_value_init(void);
void etk_value_shutdown(void);

/* Document */
Etk_Engine * etk_document_engine_get(Etk_Document *d);
Etch * etk_document_etch_get(Etk_Document *d);

/* Canvas */
Etk_Document * etk_canvas_document_get(Etk_Canvas *c);

/* Shape */
Etk_Canvas * etk_shape_canvas_get(Etk_Shape *s);
void * etk_shape_context_get(Etk_Shape *s);

/* Coord */
void etk_coord_change(const Ekeko_Object *o, Etk_Coord *dst, Etk_Coord *curr,
		Etk_Coord *prev, int coord, int length, Ekeko_Object *parent,
		const char *cevent, const char *levent, Event_Listener el);
void etk_coord_length_change(const Ekeko_Object *o, Etk_Coord *dst, Etk_Coord *curr,
		Etk_Coord *prev, int length, Ekeko_Object *parent,
		const char *levent, Event_Listener el);
#endif
