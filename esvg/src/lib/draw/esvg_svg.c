#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _flush(void *data, Eina_Rectangle *rects)
{
	ESVG *e = data;
	Eina_Inlist *l;

	return 1; 
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};
/* called when a shape of the canvas has changed size or position */
static void _shape_changed(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ESVG *svg = data;
	Eina_Rectangle r;
	
	ekeko_object_geometry_get(o, &r);
	eina_rectangle_union(&svg->shape_area, &r);
}

/* Container */
static void _child_add(ESVG_Container *container, ESVG_Element *element)
{
	
}
static void _child_remove(ESVG_Container *container, ESVG_Element *element)
{
	
}
/* Classs */
static void _constructor(ESVG_Svg *s)
{
	
}
static void _destructor(ESVG_Svg *s)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data)
{
	/* TODO make a shape_init function */
	//s->canvas = svg;
	s->object = ekeko_object_add(svg->canvas, class, data);
	s->engine.context = esvg_engine_context_new(svg);
	s->attributes.opacity.stroke_opacity = 1;
	s->attributes.opacity.fill_opacity = 1;
	ekeko_object_show(s->object);
	/* in case of resize/move a shape recalc the final canvas area */
	ekeko_object_event_callback_add(s->object, EKEKO_EVENT_MOVE, _shape_changed, svg);
	ekeko_object_event_callback_add(s->object, EKEKO_EVENT_RESIZE, _shape_changed, svg);
}
void esvg_canvas_shape_remove(ESVG_Shape *s)
{
	/* TODO remove the ekeko object */
	/* TODO remove the context */ 
}
/* TODO
void esvg_canvas_resize_register()
*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eobj_Class * esvg_svg_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		c = eobj_class_new("ESVG_Svg", ESVG_CONTAINER_CLASS,
				sizeof(ESVG_Svg), EOBJ_CONSTRUCTOR(_constructor),
				EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 * TODO change w and h 
 */
EAPI ESVG * esvg_svg_new(void)
{
	esvg_element_new(ESVG_SVG_CLASS, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_damage_add(ESVG *e, ESVG_Coord_Value x, ESVG_Coord_Value y, ESVG_Length_Value w, ESVG_Length_Value h)
{
	Eina_Rectangle r;
	
	eina_rectangle_coords_from(&r, x, y, w, h);
	ekeko_canvas_damage_add(e->canvas, &r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_render(ESVG *e)
{
	ekeko_canvas_process(e->canvas);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_svg_del(ESVG *e)
{
	
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_size_set(ESVG *e, ESVG_Length *w, ESVG_Length *h)
{
	ESVG_Length zero;
	
	/* if no canvas yet, nothing to do */
	if (!e->canvas)
		return EINA_FALSE;
	/* if no engine yet, nothing to do */
	if (e->engine_type == ESVG_ENGINE_UNKNOWN)
		return EINA_FALSE;
	/* add the background object, the reference for every relative object */
	/* if the size is relative, take the values relative to the output size */
	printf("background??\n");
	if (!e->background)
	{
		ESVG_Shape *shape;
		
		/*e->background = esvg_rect_add(e);*/
		shape = esvg_rect_shape_get(e->background);
		esvg_shape_color_set(shape, 0xffffff);
		esvg_shape_fill_set(shape, 0xffffff);
		esvg_shape_opacity_set(shape, 1.0);
	}
	zero.value = 0;
	zero.type_value = 0;
	zero.type = ESVG_LENGTH_TYPE_NUMBER;
	/* TODO should thi calculate be here? */
	esvg_length_calculate(w, e->width);
	esvg_length_calculate(h, e->height);
	
	esvg_rect_geometry_set(e->background, &zero, &zero, w, h);
	
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_output_size_set(ESVG *e, unsigned int width, unsigned int height)
{
	if (!e->canvas)
	{
		e->canvas = ekeko_canvas_new(&_canvas_class, e, EKEKO_TILER_SPLITTER, width, height);
		if (!e->canvas)
			return EINA_FALSE;
	}
	else
	{
		if ((e->width == width) && (e->height == height))
			return EINA_TRUE;
		/* we should resize the canvas size */
		printf("not supported yet!\n");
	}
	e->width = width;
	e->height = height;
	
	if (!e->background)
		return EINA_TRUE;
	
	/* if the size of the canvas is relative update the background size */
	if (esvg_length_type_is_relative(e->background->width.type))
		esvg_length_calculate(&e->background->width, e->width);
	if (esvg_length_type_is_relative(e->background->height.type))
		esvg_length_calculate(&e->background->width, e->height);
	printf("relative!!\n");
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_engine_set(ESVG *e, ESVG_Engine_Type type, void *engine_data)
{
	if (e->engine_type != ESVG_ENGINE_UNKNOWN)
		return EINA_FALSE;
	e->engine_data = engine_data;
	e->engine_type = type;
	switch (type)
	{
		case ESVG_ENGINE_CAIRO:
		e->engine = &esvg_engine_cairo;
		break;
		
		default:
		break;
	}
	return EINA_TRUE;
}

/* TODO viewport functions */
