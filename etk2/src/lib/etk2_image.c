/*
 * etk2_image.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#define ETK_IMAGE_DEBUG
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Image_Private *)((Etk_Image *)(d))->private)
struct _Etk_Image_Private
{
	struct
	{
		char *curr;
		char *prev;
		int changed;
	} file;
	Enesim_Surface *img;
};

static void _loader_callback(Enesim_Surface *s, void *data, int error)
{
	Etk_Image *i = (Etk_Image *)data;
	Etk_Image_Private *prv = PRIVATE(i);

	/* TODO add a damage to the canvas */
	printf("[Etk_Image] Image file %s loaded!!\n", prv->file.curr);
}


static void _geometry_calc(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Image *i = (Etk_Image *)o;
	Etk_Image_Private *prv = PRIVATE(i);
	Eina_Rectangle geom;
	Etk_Coord x, y, w, h;

	/* TODO compute the final geometry multiplying by the context matrix */
	etk_square_coords_get((Etk_Square *)i, &x, &y, &w, &h);
	eina_rectangle_coords_from(&geom, x.final, y.final, w.final,
			h.final);

#ifdef ETK_DEBUG
	printf("[Etk_Image] Setting geometry of size %d %d %d %d\n",
			x.final, y.final, w.final, h.final);
#endif
	ekeko_renderable_geometry_set((Ekeko_Renderable *)i, &geom);
}

static void _file_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Image *i = (Etk_Image *)o;
	Etk_Image_Private *prv = PRIVATE(o);

	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	/* call emage to load an image */
	emage_load_async(em->curr->value.string_value, &prv->img, _loader_callback, i, NULL);
}

static void _render(Etk_Shape *s, Etk_Engine *func, Etk_Surface *surface, Etk_Context *context)
{
	printf("[Etk_Image] Trying to render the image\n");
}

static void _ctor(void *instance)
{
	Etk_Image *i;
	Etk_Image_Private *prv;

	i = (Etk_Image*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_image_type_get(), instance);
	i->parent.parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_IMAGE_FILE_CHANGED, _file_change, EINA_FALSE, NULL);
}

static void _dtor(void *image)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, ETK_TYPE_ANIMATION))
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Property_Id ETK_IMAGE_FILE;

EAPI Ekeko_Type *etk_image_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_IMAGE, sizeof(Etk_Image),
				sizeof(Etk_Image_Private), etk_square_type_get(),
				_ctor, _dtor, _appendable);
		ETK_IMAGE_FILE = TYPE_PROP_DOUBLE_ADD(type, "file", PROPERTY_STRING,
				OFFSET(Etk_Image_Private, file.curr), OFFSET(Etk_Image_Private, file.prev),
				OFFSET(Etk_Image_Private, file.changed));
	}

	return type;
}

EAPI Etk_Image * etk_image_new(Etk_Canvas *c)
{
	Etk_Image *r;

	r = ekeko_type_instance_new(etk_image_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)r);

	return r;
}

EAPI void etk_image_file_set(Etk_Image *i, const char *file)
{
	Ekeko_Value v;

	ekeko_value_str_from(&v, file);
	ekeko_object_property_value_set((Ekeko_Object *)i, "file", &v);
}

EAPI const char * etk_image_file_get(Etk_Image *i)
{
	Etk_Image_Private *prv;

	prv = PRIVATE(i);
	return prv->file.curr;
}
