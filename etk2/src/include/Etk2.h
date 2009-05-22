#ifndef _ETK2_H
#define _ETK2_H

#include "Eina.h"
#include "Ekeko.h"
#include "Enesim.h"
#include "Ecore.h"
#include "Etch.h"

typedef struct _Etk_Document Etk_Document;
typedef struct _Etk_Canvas Etk_Canvas;
typedef struct _Etk_Shape Etk_Shape;
typedef struct _Etk_Animation Etk_Animation;
typedef struct _Etk_Animation_Basic Etk_Animation_Basic;
typedef struct _Etk_Animation_Matrix Etk_Animation_Matrix;
typedef struct _Etk_Square Etk_Square;
typedef struct _Etk_Rect Etk_Rect;
typedef struct _Etk_Image Etk_Image;
typedef struct _Etk_Filter Etk_Filter;
typedef struct _Etk_Filter_Effect Etk_Filter_Effect;
/* filter effects */
typedef struct _Etk_Fe_Displace Etk_Fe_Displace;


/* TODO use this generic names */
typedef struct _Etk_Engine Etk_Engine;
typedef void Etk_Context;
typedef void Etk_Surface;

EAPI int etk_init(void);
EAPI void etk_loop(void);
EAPI int etk_shutdown(void);

#include "etk2_coord.h"
#include "etk2_color.h"
#include "etk2_value.h"
#include "etk2_document.h"
#include "etk2_canvas.h"
#include "etk2_shape.h"
#include "etk2_square.h"
#include "etk2_rect.h"
#include "etk2_image.h"

#include "etk2_animation.h"
#include "etk2_anim_basic.h"
#include "etk2_anim_matrix.h"

#include "etk2_filter.h"
#include "etk2_filter_effect.h"
#include "etk2_fe_displace.h"

#include "etk2_widget.h"
#include "etk2_button.h"
#include "etk2_label.h"

#endif
