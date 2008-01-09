#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* TODO move this to object.c? */
static void _object_changed(Ekeko_Canvas *c, Ekeko_Object *o)
{
	/* check if the object has changed */
	/* call the pre process function if so */
}

static void _objects_changed(Ekeko_Canvas *c)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)c->objects; l; l = l->next)
	{
		Ekeko_Object *o;

		o = (Ekeko_Object *)l;
		_object_changed(c, o); 
	}
}
static void _damages_add(Ekeko_Canvas *c)
{
	/* add the rectangles to the tiler */

}
static void _obscures_remove(Ekeko_Canvas *c)
{
	/* remove the rectangles from the tiler */
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_canvas_new(int type, int w, int h)
{
	Ekeko_Canvas *c;

	c = calloc(1, sizeof(Ekeko_Canvas));
	c->tiler = ekeko_tiler_new(type, w, h);
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_process(Ekeko_Canvas *c)
{
	Ekeko_Rectangle *redraws;

	/* 1. check changed objects */
	_objects_changed(c);
	/* 2. add damages */
	_damages_add(c);
	/* 3. remove obscures */
	_obscures_remove(c);
	/* 4. get all rectangles to redraw */
	//redraws = ekeko_tiler_rects_get(c->tiler);
	/* 5. process al objects that intersect with the rects */
	/* 6. delete all objects that should be deleted */
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_data_set(Ekeko_Canvas *c, void *data)
{
	c->data = data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * ekeko_canvas_data_get(Ekeko_Canvas *c)
{
	return c->data;
}
