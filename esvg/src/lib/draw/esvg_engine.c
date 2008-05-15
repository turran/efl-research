#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* Classs */
static void _constructor(ESVG_Engine *e)
{
	printf("engine constructor\n");
	e->h = 0;
	e->w = 0;
}
static void _destructor(ESVG_Engine *e)
{
	printf("engine destructor\n");
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void * esvge_context_new(ESVG_Engine *e)
{
	return e->func->context_new(e->data);
}

void esvge_context_free(ESVG_Engine *e, void *context)
{
	e->func->context_free(e->data, context);
}

void esvge_rect_draw(ESVG_Engine *e, void *context, ESVG_Rect *r, Eina_Rectangle *rclip)
{
	e->func->rect_draw(e->data, context, r, rclip);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eobj_Class * esvg_engine_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		c = eobj_class_new("ESVG_Engine", EOBJ_OBJECT_CLASS,
				sizeof(ESVG_Engine), EOBJ_CONSTRUCTOR(_constructor),
				EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_engine_output_size_get(ESVG_Engine *e, unsigned int *width, unsigned int *height)
{
	if (width) *width = e->w;
	if (height) *height = e->h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_engine_output_size_set(ESVG_Engine *e, unsigned int width, unsigned int height)
{
	if ((e->w == width) && (e->h == height))
		return EINA_TRUE;
	e->w = width;
	e->h = height;
	//eobj_object_notify()
	return EINA_TRUE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_engine_set(ESVG_Engine *e, ESVG_Engine_Type type, void *engine_data)
{
	/* engine already set */
	if (e->type != ESVG_ENGINE_UNKNOWN)
		return EINA_FALSE;
	e->data = engine_data;
	e->type = type;
	switch (type)
	{
		case ESVG_ENGINE_CAIRO:
		e->func = &esvg_engine_cairo;
		break;
		
		default:
		break;
	}
	return EINA_TRUE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI ESVG_Engine * esvg_engine_new(void)
{
	return ESVG_ENGINE(eobj_object_new(ESVG_ENGINE_CLASS, NULL));
}

