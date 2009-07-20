#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* Element */
static void _ancestor_cb(Eobj_Object *o, const char *pname, void *data)
{
	ESVG_Svg *ancestor;
	ESVG_Shape *s = ESVG_SHAPE(o);
	
	ancestor = esvg_element_ancestor_get(ESVG_ELEMENT(o));
	/* create a new ekeko object */
	if (ancestor)
	{
		
		if (s->object)
		{
			/* ekeko_object_delete(s->object); */
		}
		s->object = ekeko_object_add(ancestor->canvas, s->object_class, s);
	}
	/* recreate the attributes based on the ancestor */
	printf("ancestor cb called\n");
}
/* Classs */
static void _constructor(ESVG_Shape *s)
{
	/* default values */
	s->object = NULL;
	/* register the notification callbacks */
	eobj_object_notification_callback_add(EOBJ_OBJECT(s), "ancestor", _ancestor_cb, NULL);
}
static void _destructor(ESVG_Shape *s)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
/* called when a parent has been set */
void esvg_shape_geometry_set(ESVG_Shape *s, ESVG_Coord_Value x, ESVG_Coord_Value y, ESVG_Length_Value width, ESVG_Length_Value height)
{
	/* TODO round values to up for positive or down for negatives */
	//printf("%d %d %d %d\n", x, y, width, height);
	/* TODO the geometry has to be calculated adding the stroke
	 * and the transformations, etc
	 */
	ekeko_object_move(s->object, x, y);
	ekeko_object_resize(s->object, width, height);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eobj_Class * esvg_shape_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		c = eobj_class_new("ESVG_Shape", ESVG_ELEMENT_CLASS,
				sizeof(ESVG_Shape), EOBJ_CONSTRUCTOR(_constructor),
				EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;
}
/* FIXME all this attributes go to esvg_element.c */
/* Core Attributes */
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_id_set(ESVG_Shape *s, char *id)
{
	if (s->attributes.core.id)
		free(s->attributes.core.id);
	s->attributes.core.id = strdup(id);
}

/* Paint attributes */
/**
 * To be documented
 * FIXME: To be fixed
 */
/* TODO what'sthe difference between color and fill? */
EAPI void esvg_shape_color_set(ESVG_Shape *s, ESVG_Color color)
{
	s->attributes.paint.color = color;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_fill_set(ESVG_Shape *s, ESVG_Color fill)
{
	s->attributes.paint.fill = fill;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_fill_rule_set(ESVG_Shape *s, ESVG_Fill_Rule fill_rule)
{
	s->attributes.paint.fill_rule = fill_rule;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_stroke_set(ESVG_Shape *s, ESVG_Color color)
{
	s->attributes.paint.stroke = color;
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
/* TODO This opacity should change both? */
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_opacity_set(ESVG_Shape *s, double opacity)
{
	esvg_shape_stroke_opacity_set(s, opacity);
	esvg_shape_fill_opacity_set(s, opacity);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_stroke_opacity_set(ESVG_Shape *s, double opacity)
{
	s->attributes.opacity.stroke_opacity = opacity;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_shape_fill_opacity_set(ESVG_Shape *s, double opacity)
{
	s->attributes.opacity.fill_opacity = opacity;
}

