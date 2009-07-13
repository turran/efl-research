/*
 * eon_engine.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Engine_Private
{

};

static Eina_Hash *_engines = NULL;

static void shape_color_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{

}

static void shape_rop_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{

}


static void _child_append_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;

	/* add callbacks to the different types */
	if (ekeko_type_instance_is_of(e->target, EON_TYPE_SHAPE))
	{
		ekeko_event_listener_add(o, EON_SHAPE_COLOR_CHANGED, shape_color_change, EINA_FALSE, NULL);
		ekeko_event_listener_add(o, EON_SHAPE_ROP_CHANGED, shape_rop_change, EINA_FALSE, NULL);
		printf("%s appended to the document\n", ekeko_object_type_name_get(e->target));
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_engine_init(void)
{
	_engines = eina_hash_string_superfast_new(NULL);
	/* now the canvas engines */
#ifdef BUILD_ENGINE_SDL
	engine_sdl_init();
#endif
#ifdef BUILD_ENGINE_BUFFER
	engine_buffer_init();
#endif
}

void eon_engine_shutdown(void)
{
	/* TODO remove the hash */
}

void eon_engine_register(const char *name, Eon_Engine_New n)
{
	eina_hash_add(_engines, name, n);
}

Eon_Engine * eon_engine_get(const char *name)
{
	Ekeko_Object *o;
	Ekeko_Type *t;
	Eon_Engine_New n;

	n = eina_hash_find(_engines, name);
	if (!n)
		return NULL;
	/* get the engine's type */
	t = n();
	if (!t)
		return NULL;
	/* create the instance */
	o = ekeko_type_instance_new(t);
	if (!ekeko_type_instance_is_of(o, EON_TYPE_ENGINE))
	{
		ekeko_type_instance_delete(o);
	}
	return (Eon_Engine *)o;
}

void eon_engine_ref(Eon_Engine *e, Eon_Document *d)
{
	/* register the needed callbacks */
	ekeko_event_listener_add((Ekeko_Object *)d, EVENT_OBJECT_APPEND, _child_append_cb, EINA_TRUE, e);
}

void eon_engine_unref(Eon_Engine *e, Eon_Document *d)
{
	/* unregister every callback */
}

static void _ctor(void *instance)
{
	Eon_Engine *e;
	Eon_Engine_Private *prv;

	e = (Eon_Engine*) instance;
	e->private = prv = ekeko_type_instance_private_get(eon_engine_type_get(), instance);
}

static void _dtor(void *instance)
{

}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Type *eon_engine_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_ENGINE, sizeof(Eon_Engine),
				sizeof(Eon_Engine_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;

}

EAPI void * eon_engine_document_create(Eon_Engine *e, Eon_Document *d)
{
	return e->document_create(d);
}


EAPI Eina_Bool eon_engine_canvas_blit(Eon_Engine *e, void *sc, Eina_Rectangle *sr, void *c, Eina_Rectangle *r)
{
	return e->canvas_blit(sc, sr, c, r);
}

EAPI Eina_Bool eon_engine_canvas_flush(Eon_Engine *e, void *c, Eina_Rectangle *r)
{
	return e->canvas_flush(c, r);
}

EAPI void * eon_engine_canvas_create(Eon_Engine *e, Eon_Canvas *c, Eina_Bool root, uint32_t w, uint32_t h)
{
	return e->canvas_create(c, root, w, h);
}

EAPI void * eon_engine_rect_create(Eon_Engine *e, Eon_Rect *r)
{
	return e->rect_create(r);
}

EAPI void eon_engine_rect_render(Eon_Engine *e, void *r, void *c, Eina_Rectangle *clip)
{
	e->rect_render(r, c, clip);

}

EAPI void * eon_engine_circle_create(Eon_Engine *e, Eon_Circle *c)
{
	return e->circle_create(c);
}

EAPI void eon_engine_circle_render(Eon_Engine *e, void *r, void *c, Eina_Rectangle *clip)
{
	e->circle_render(r, c, clip);

}

EAPI void * eon_engine_polygon_create(Eon_Engine *e, Eon_Polygon *p)
{
	return e->polygon_create(p);
}

EAPI void eon_engine_polygon_point_add(Eon_Engine *e, void *pd, int x, int y)
{
	e->polygon_point_add(pd, x, y);
}

EAPI void eon_engine_polygon_render(Eon_Engine *e, void *p, void *c, Eina_Rectangle *clip)
{
	e->polygon_render(p, c, clip);
}

EAPI void * eon_engine_image_create(Eon_Engine *e, Eon_Paint *p)
{
	return e->image_create((Eon_Image *)p);
}

EAPI void eon_engine_image_setup(Eon_Engine *e, void *engine_data, Eon_Shape *s)
{
	e->image_setup(engine_data, s);
}

EAPI void eon_engine_image_delete(Eon_Engine *e, void *engine_data)
{
	e->image_delete(engine_data);
}

EAPI void eon_engine_debug_rect(Eon_Engine *e, void *c, uint32_t color, int x,  int y, int w, int h)
{
	e->debug_rect(c, color, x, y, w, h);
}

