/*
 * eon_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
//#define EON_DEBUG

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

/* Type names */
#define EON_TYPE_DOCUMENT "Eon_Document"
#define EON_TYPE_CANVAS "Eon_Canvas"
#define EON_TYPE_RECT "Eon_Rect"
#define EON_TYPE_IMAGE "Eon_Image"
#define EON_TYPE_POLYGON "Eon_Polygon"
#define EON_TYPE_CIRCLE "Eon_Circle"
#define EON_TYPE_ENGINE "Eon_Engine"
#define EON_TYPE_SHAPE "Eon_Shape"


#define EON_TYPE_ANIMATION "Eon_Animation"
#define EON_TYPE_ANIMATION_BASIC "Eon_Animation_Basic"
#define EON_TYPE_ANIMATION_MATRIX "Eon_Animation_Matrix"

#define EON_TYPE_FILTER "Eon_Filter"
#define EON_TYPE_FILTER_EFFECT "Eon_Filter_Effect"

/* Engine! */
typedef void * Eon_Engine_Surface;
void eon_engine_init(void);
void eon_engine_shutdown(void);
void eon_engine_register(const char *name, Eon_Engine_New n);
Eon_Engine * eon_engine_get(const char *name);

/* Enesim engine */
Ekeko_Type * eon_engine_enesim_type_get(void);

/* SDL engine */
void engine_sdl_init(void);

/* Ekeko_Value */
void eon_value_init(void);
void eon_value_shutdown(void);

/* Document */
Eon_Engine * eon_document_engine_get(Eon_Document *d);
Etch * eon_document_etch_get(Eon_Document *d);

/* Canvas */
Eon_Document * eon_canvas_document_get(Eon_Canvas *c);

/* Shape */
Eon_Canvas * eon_shape_canvas_get(Eon_Shape *s);
void * eon_shape_engine_data_get(Eon_Shape *s);
void eon_shape_engine_data_set(Eon_Shape *s, void *engine_data);
void eon_shape_change(Eon_Shape *s);

/* Coord */
void eon_coord_change(const Ekeko_Object *o, Eon_Coord *dst, Eon_Coord *curr,
		Eon_Coord *prev, int coord, int length, Ekeko_Object *parent,
		const char *cevent, const char *levent, Event_Listener el);
void eon_coord_length_change(const Ekeko_Object *o, Eon_Coord *dst, Eon_Coord *curr,
		Eon_Coord *prev, int length, Ekeko_Object *parent,
		const char *levent, Event_Listener el);
#endif
