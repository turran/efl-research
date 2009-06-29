#ifndef _EON_H
#define _EON_H

#include "Eina.h"
#include "Ekeko.h"
#include "Enesim.h"
#include "Ecore.h"
#include "Etch.h"

typedef struct _Eon_Document Eon_Document;
typedef struct _Eon_Canvas Eon_Canvas;
typedef struct _Eon_Shape Eon_Shape;
typedef struct _Eon_Animation Eon_Animation;
typedef struct _Eon_Animation_Basic Eon_Animation_Basic;
typedef struct _Eon_Animation_Matrix Eon_Animation_Matrix;
typedef struct _Eon_Square Eon_Square;
typedef struct _Eon_Rect Eon_Rect;
typedef struct _Eon_Image Eon_Image;
typedef struct _Eon_Circle Eon_Circle;
typedef struct _Eon_Polygon Eon_Polygon;
typedef struct _Eon_Filter Eon_Filter;
/* filter effects */
typedef struct _Eon_Fe_Displace Eon_Fe_Displace;


/* TODO use this generic names */
typedef struct _Eon_Engine Eon_Engine;
/* TODO remove this */
typedef void Eon_Context;
typedef void Eon_Surface;

EAPI int eon_init(void);
EAPI void eon_loop(void);
EAPI int eon_shutdown(void);

#include "eon_engine.h"

#include "eon_coord.h"
#include "eon_color.h"
#include "eon_value.h"
#include "eon_document.h"
#include "eon_canvas.h"
#include "eon_shape.h"
#include "eon_square.h"
#include "eon_rect.h"
#include "eon_image.h"
#include "eon_polygon.h"
#include "eon_circle.h"

#include "eon_animation.h"
#include "eon_anim_basic.h"
#include "eon_anim_matrix.h"

#include "eon_filter.h"
#include "eon_fe_displace.h"

#include "eon_widget.h"
#include "eon_button.h"
#include "eon_label.h"


#endif
