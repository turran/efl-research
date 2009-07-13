/*
 * eon_image.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "Emage.h"
#include "eon_private.h"
#define EON_IMAGE_DEBUG 0
/* TODO
 * + create a temporary image when the size has changed of size of the image itself
 * with some pattern in to inform that the image is loading on the background
 * + whenever a shape references this paint object we should add a notifier
 * to inform that the image was succesfully async loaded so the shape can
 * show the image instead of the pattern
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Image_Private *)((Eon_Image *)(d))->private)
struct _Eon_Image_Private
{
	struct
	{
		char *curr;
		char *prev;
		int changed;
	} file;
	struct {
		Enesim_Surface *img;
		Eina_Bool loaded;
	} src;
	/* TODO add l, t, r, b */
};

static void _loader_callback(Enesim_Surface *s, void *data, int error)
{
	Eon_Image *i = (Eon_Image *)data;
	Eon_Image_Private *prv = PRIVATE(i);

	/* TODO check for error */
	if (error)
	{
		printf("[Eon_Image] Error %d %s\n", error, eina_error_msg_get(error));
	}
	else
	{
		Ekeko_Value v;

		printf("Image loaded, setting the flag!\n");
		ekeko_value_bool_from(&v, EINA_TRUE);
		ekeko_object_property_value_set((Ekeko_Object *)i, "loaded", &v);
	}
}

static void _file_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Image *i = (Eon_Image *)o;
	Eon_Image_Private *prv = PRIVATE(o);

	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	prv->src.loaded = EINA_FALSE;
	/* call emage to load an image */
	emage_load_async(em->curr->value.string_value, &prv->src.img, _loader_callback, i, NULL);
}

static void _ctor(void *instance)
{
	Eon_Image *i;
	Eon_Image_Private *prv;

	i = (Eon_Image*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_image_type_get(), instance);
	i->parent.create = eon_engine_image_create;
	i->parent.setup = eon_engine_image_setup;
	i->parent.delete = eon_engine_image_delete;
	ekeko_event_listener_add((Ekeko_Object *)i, EON_IMAGE_FILE_CHANGED, _file_change, EINA_FALSE, NULL);
}

static void _dtor(void *image)
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
Eina_Bool eon_image_loaded(Eon_Image *i)
{
	Eon_Image_Private *prv;

	prv = PRIVATE(i);
	return prv->src.loaded;
}

Eina_Bool eon_image_size_get(Eon_Image *i, int *w, int *h)
{
	Eon_Image_Private *prv = PRIVATE(i);

	if (!prv->src.loaded)
		return EINA_FALSE;

	enesim_surface_size_get(prv->src.img, w, h);
	return EINA_TRUE;
}

Enesim_Surface * eon_image_surface_get(Eon_Image *i)
{
	Eon_Image_Private *prv = PRIVATE(i);

	return prv->src.img;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_IMAGE_FILE;
Ekeko_Property_Id EON_IMAGE_LOADED;

EAPI Ekeko_Type *eon_image_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_IMAGE, sizeof(Eon_Image),
				sizeof(Eon_Image_Private), eon_paint_type_get(),
				_ctor, _dtor, _appendable);
		EON_IMAGE_FILE = TYPE_PROP_DOUBLE_ADD(type, "file", PROPERTY_STRING,
				OFFSET(Eon_Image_Private, file.curr), OFFSET(Eon_Image_Private, file.prev),
				OFFSET(Eon_Image_Private, file.changed));
		EON_IMAGE_LOADED = TYPE_PROP_SINGLE_ADD(type, "loaded", PROPERTY_BOOL, OFFSET(Eon_Image_Private, src.loaded));
	}

	return type;
}

EAPI Eon_Image * eon_image_new(Eon_Canvas *c)
{
	Eon_Image *i;

	i = ekeko_type_instance_new(eon_image_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)i);

	return i;
}

EAPI void eon_image_file_set(Eon_Image *i, const char *file)
{
	Ekeko_Value v;

	ekeko_value_str_from(&v, file);
	ekeko_object_property_value_set((Ekeko_Object *)i, "file", &v);
}

EAPI const char * eon_image_file_get(Eon_Image *i)
{
	Eon_Image_Private *prv;

	prv = PRIVATE(i);
	return prv->file.curr;
}
