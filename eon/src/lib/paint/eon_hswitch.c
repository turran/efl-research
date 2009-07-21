/*
 * eon_hswitch.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "Emage.h"
#include "eon_private.h"

/* TODO
 * instead of handling only two images we can do the logic for a list
 * of images at the end the renderer only needs two but we can fake that :)
 */

#define EON_HSWITCH_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Hswitch_Private *)((Eon_Hswitch *)(d))->private)

typedef struct _Eon_Hswitch_Image
{
	Ekeko_Object parent;
	Enesim_Surface *s;
	char *file;
} Eon_Hswitch_Image;

struct _Eon_Hswitch_Private
{
	struct
	{
		Enesim_Surface *src;
		char *file;
		Eina_Bool loaded;
	} left, right;
	float step;
};

static void _loader_callback(Enesim_Surface *s, void *data, int error, Eina_Bool left)
{
	Eon_Hswitch *i = (Eon_Hswitch *)data;
	Eon_Hswitch_Private *prv = PRIVATE(i);

	/* TODO check for error */
	if (error)
	{
		printf("[Eon_Hswitch] Error %d %s\n", error, eina_error_msg_get(error));
	}
	/* Right now when the hswitch is succesfully loaded we change a flag
	 * and an event is triggered. So a shape that references this paint object
	 * should register to this signal and mark itself as dirty whenever the
	 * signal is emitted.
	 */
	else
	{
		Ekeko_Value v;
		ekeko_value_bool_from(&v, EINA_TRUE);
		if (left)
			ekeko_object_property_value_set((Ekeko_Object *)i, "lloaded", &v);
		else
			ekeko_object_property_value_set((Ekeko_Object *)i, "rloaded", &v);
	}
}

static void _lloader_callback(Enesim_Surface *s, void *data, int error)
{
	_loader_callback(s, data, error, EINA_TRUE);
}

static void _rloader_callback(Enesim_Surface *s, void *data, int error)
{
	_loader_callback(s, data, error, EINA_FALSE);
}

static void _file_change(const Ekeko_Object *o, Ekeko_Event *e, Eina_Bool left)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Hswitch *i = (Eon_Hswitch *)o;
	Eon_Hswitch_Private *prv = PRIVATE(o);

	if (left)
	{
		prv->left.loaded = EINA_FALSE;
		if (prv->left.src)
				enesim_surface_delete(prv->left.src);
		eon_cache_image_load(em->curr->value.string_value, &prv->left.src, ENESIM_FORMAT_ARGB8888, _lloader_callback, i, NULL);
	}
	else
	{
		prv->right.loaded = EINA_FALSE;
		if (prv->right.src)
			enesim_surface_delete(prv->right.src);
		eon_cache_image_load(em->curr->value.string_value, &prv->right.src, ENESIM_FORMAT_ARGB8888, _rloader_callback, i, NULL);
	}
}

static void _lfile_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	_file_change(o, e, EINA_TRUE);
}

static void _rfile_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	_file_change(o, e, EINA_FALSE);
}

static void _ctor(void *instance)
{
	Eon_Hswitch *i;
	Eon_Hswitch_Private *prv;

	i = (Eon_Hswitch*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_hswitch_type_get(), instance);
	i->parent.create = eon_engine_hswitch_create;
	i->parent.setup = eon_engine_hswitch_setup;
	i->parent.delete = eon_engine_hswitch_delete;
	ekeko_event_listener_add((Ekeko_Object *)i, EON_HSWITCH_LFILE_CHANGED, _lfile_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_HSWITCH_RFILE_CHANGED, _rfile_change, EINA_FALSE, NULL);
}

static void _dtor(void *hswitch)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, EON_TYPE_ANIMATION))
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool eon_hswitch_left_loaded(Eon_Hswitch *i)
{
	Eon_Hswitch_Private *prv;

	prv = PRIVATE(i);
	return prv->left.loaded;
}

Eina_Bool eon_hswitch_right_loaded(Eon_Hswitch *i)
{
	Eon_Hswitch_Private *prv;

	prv = PRIVATE(i);
	return prv->right.loaded;
}

float eon_hswitch_step_get(Eon_Hswitch *hs)
{
	Eon_Hswitch_Private *prv = PRIVATE(hs);
	return prv->step;
}

Enesim_Surface * eon_hswitch_right_get(Eon_Hswitch *hs)
{
	Eon_Hswitch_Private *prv = PRIVATE(hs);

	return prv->right.src;
}

Enesim_Surface * eon_hswitch_left_get(Eon_Hswitch *hs)
{
	Eon_Hswitch_Private *prv = PRIVATE(hs);

	return prv->left.src;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_HSWITCH_LFILE;
Ekeko_Property_Id EON_HSWITCH_STEP;
Ekeko_Property_Id EON_HSWITCH_RFILE;
Ekeko_Property_Id EON_HSWITCH_LLOADED;
Ekeko_Property_Id EON_HSWITCH_RLOADED;

EAPI Ekeko_Type *eon_hswitch_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_HSWITCH, sizeof(Eon_Hswitch),
				sizeof(Eon_Hswitch_Private), eon_paint_type_get(),
				_ctor, _dtor, _appendable);

		EON_HSWITCH_LFILE = TYPE_PROP_SINGLE_ADD(type, "lfile", PROPERTY_STRING, OFFSET(Eon_Hswitch_Private, left.file));
		EON_HSWITCH_RFILE = TYPE_PROP_SINGLE_ADD(type, "rfile", PROPERTY_STRING, OFFSET(Eon_Hswitch_Private, right.file));
		EON_HSWITCH_STEP = TYPE_PROP_SINGLE_ADD(type, "step", PROPERTY_FLOAT, OFFSET(Eon_Hswitch_Private, step));
		EON_HSWITCH_LLOADED = TYPE_PROP_SINGLE_ADD(type, "lloaded", PROPERTY_BOOL, OFFSET(Eon_Hswitch_Private, left.loaded));
		EON_HSWITCH_RLOADED = TYPE_PROP_SINGLE_ADD(type, "rloaded", PROPERTY_BOOL, OFFSET(Eon_Hswitch_Private, right.loaded));

	}

	return type;
}

EAPI Eon_Hswitch * eon_hswitch_new(Eon_Canvas *c)
{
	Eon_Hswitch *hs;

	hs = ekeko_type_instance_new(eon_hswitch_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)hs);

	return hs;
}

EAPI void eon_hswitch_left_set(Eon_Hswitch *hs, const char *file)
{
	Ekeko_Value v;

	ekeko_value_str_from(&v, file);
	ekeko_object_property_value_set((Ekeko_Object *)hs, "rfile", &v);
}

EAPI void eon_hswitch_right_set(Eon_Hswitch *hs, const char *file)
{
	Ekeko_Value v;

	ekeko_value_str_from(&v, file);
	ekeko_object_property_value_set((Ekeko_Object *)hs, "lfile", &v);
}

