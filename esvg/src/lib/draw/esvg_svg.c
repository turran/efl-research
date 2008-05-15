#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* Ekeko Canvas */
static int _flush(void *data, Eina_Rectangle *rects)
{
	ESVG_Svg *e = data;
	Eina_Inlist *l;

	return 1; 
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};
/* called when a shape of the canvas has changed size or position */
#if 0
static void _shape_changed(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ESVG_Svg *e = data;
	Eina_Rectangle r;
	
	ekeko_object_geometry_get(o, &r);
	eina_rectangle_union(&svg->shape_area, &r);
}
#endif
/* Container */
static void _child_add(ESVG_Container *container, ESVG_Element *element)
{
	esvg_element_ancestor_set(element, ESVG_SVG(container));
	printf("adding child\n");
}
static void _child_remove(ESVG_Container *container, ESVG_Element *element)
{
	esvg_element_ancestor_set(element, NULL);
	printf("removing child\n");
}
/* Classs */
static void _constructor(ESVG_Svg *s)
{
	s->engine = NULL;
	s->canvas = ekeko_canvas_new(&_canvas_class, s, EKEKO_TILER_SPLITTER, 0, 0);
	ESVG_CONTAINER(s)->child_add = _child_add;
	ESVG_CONTAINER(s)->child_remove = _child_remove;
	
	printf("svg constructor\n");
}
static void _destructor(ESVG_Svg *s)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
#if 0
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
#endif
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
 */
EAPI ESVG_Element * esvg_svg_new(void)
{
	return esvg_element_new(ESVG_SVG_CLASS, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_x_set(ESVG_Svg *s, ESVG_Length *x)
{
	s->x.base = *x;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_y_set(ESVG_Svg *s, ESVG_Length *y)
{
	s->y.base = *y;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_width_set(ESVG_Svg *s, ESVG_Length *w)
{
	s->width.base = *w;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_height_set(ESVG_Svg *s, ESVG_Length *h)
{
	s->height.base = *h;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI ESVG_Element * esvg_svg_element_by_id_get(ESVG_Svg *s, const char *id)
{
	/* iterate over the childs to get the element with this id */
	/*
	 * esvg_container_for_each_data(s, callback, data);
	 */
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_redraw_force(ESVG_Svg *s)
{
	/* process the canvas */
	/* in case we have an engine set */
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_svg_engine_set(ESVG_Svg *s, ESVG_Engine *engine)
{
	/* only can be set once ? */
	s->engine = engine;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI ESVG_Engine * esvg_svg_engine_get(ESVG_Svg *s)
{
	/* can only be set once ? */
	return s->engine;
}
/* svg element interface */
#if 0
void          unsuspendRedraw ( in unsigned long suspend_handle_id )
                  raises( DOMException );
  void          unsuspendRedrawAll (  );
  void          forceRedraw (  );
  void          pauseAnimations (  );
  void          unpauseAnimations (  );
  boolean       animationsPaused (  );
  float         getCurrentTime (  );
  void          setCurrentTime ( in float seconds );
  NodeList      getIntersectionList ( in SVGRect rect, in SVGElement referenceElement );
  NodeList      getEnclosureList ( in SVGRect rect, in SVGElement referenceElement );
  boolean       checkIntersection ( in SVGElement element, in SVGRect rect );
  boolean       checkEnclosure ( in SVGElement element, in SVGRect rect );
  void          deselectAll (  );
  SVGNumber              createSVGNumber (  );
  SVGLength              createSVGLength (  );
  SVGAngle               createSVGAngle (  );
  SVGPoint               createSVGPoint (  );
  SVGMatrix              createSVGMatrix (  );
  SVGRect                createSVGRect (  );
  SVGTransform           createSVGTransform (  );
  SVGTransform     createSVGTransformFromMatrix ( in SVGMatrix matrix );
  Element         getElementById ( in DOMString elementId );
#endif
/* old code */
#if 0
typedef struct _ESVG ESVG;
  
EAPI void esvg_damage_add(ESVG *e, ESVG_Coord_Value x, ESVG_Coord_Value y, ESVG_Length_Value w, ESVG_Length_Value h);
EAPI void esvg_free(ESVG *e);
EAPI void esvg_render(ESVG *e);
EAPI void esvg_free(ESVG *e);
EAPI Eina_Bool esvg_size_set(ESVG *e, ESVG_Length *w, ESVG_Length *h);


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
#endif
