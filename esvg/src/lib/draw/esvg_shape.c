#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_shape_geometry_set(ESVG_Shape *s, ESVG_Coord x, ESVG_Coord y, ESVG_Length width, ESVG_Length height)
{
	/* TODO round values to up for positive or down for negatives */
	//printf("%d %d %d %d\n", x, y, width, height);
	ekeko_object_move(s->object, x, y);
	ekeko_object_resize(s->object, width, height);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/* Core Attributes */
void esvg_shape_id_set(ESVG_Shape *s, char *id)
{
	if (s->attributes.core.id)
		free(s->attributes.core.id);
	s->attributes.core.id = strdup(id);
}

/* Paint attributes */
void esvg_shape_color_set(ESVG_Shape *s, ESVG_Color color)
{
	
}

void esvg_shape_fill_set(ESVG_Shape *s, ESVG_Color fill)
{
	
}

void esvg_shape_fill_rule_set(ESVG_Shape *s, ESVG_Fill_Rule fill_rule)
{
	
}
void esvg_shape_stroke_set(ESVG_Shape *s, ESVG_Color color)
{
	
}
/* TODO dash array */
#if 0
char stroke_dash_array;
ESVG_Length stroke_dashoffset;
ESVG_Stroke_Line_Cap stroke_linecap;
ESVG_Stroke_Line_Join stroke_linejoin;	
double stroke_miterlimit; /* >= 1 */
ESVG_Length stroke_width;
ESVG_Interpolation color_interpolation;
ESVG_Interpolation color_interpolation_rendering;
#endif
/* Opacity Attributes */
void esvg_shape_opacity_set(ESVG_Shape *s, double opacity)
{
	
}
void esvg_shape_stroke_opacity_set(ESVG_Shape *s, double opacity)
{
	
}
void esvg_shape_fill_opacity_set(ESVG_Shape *s, double opacity)
{
	
}

