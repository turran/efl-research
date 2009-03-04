/*
 * etk2_image.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#define ETK_IMAGE_DEBUG 0
/* TODO
 * + create a temporary image when the size has changed of size of the image itself
 * with some pattern in to inform that the image is loading on the background
 */
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
	struct {
		int w;
		int h;
		Enesim_Surface *img;
		Eina_Bool loaded;
	} src;
	Enesim_Matrix matrix;
	Enesim_Matrix inverse;
};

static void _loader_callback(Enesim_Surface *s, void *data, int error)
{
	Etk_Canvas *c;
	Etk_Image *i = (Etk_Image *)data;
	Etk_Image_Private *prv = PRIVATE(i);
	Eina_Rectangle r;

	/* TODO check for error */
	if (error)
	{
		printf("[Etk_Image] Error %d\n", error);
	}
	else
	{
		c = etk_shape_canvas_get((Etk_Shape *)i);
		ekeko_renderable_geometry_get((Ekeko_Renderable *)i, &r);
		printf("[Etk_Image] Image file %s loaded and will be displayed in %p %d %d %d %d!!\n", prv->file.curr, c, r.x, r.y, r.w, r.h);
		/* add a damage to the canvas to force a redraw of the image */
		ekeko_canvas_damage_add((Ekeko_Canvas *)c, &r);
		prv->src.loaded = EINA_TRUE;
	}
}

static void _geometry_calc(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Image *i = (Etk_Image *)o;
	Etk_Image_Private *prv = PRIVATE(i);
	Eina_Rectangle r;
	Etk_Coord x, y, w, h;
	Enesim_Quad q;
	float x1, y1, x2, y2, x3, y3, x4, y4;

	/* TODO check that the matrix is not identity */
	/* compute the final geometry multiplying by the context matrix */
	etk_square_coords_get((Etk_Square *)i, &x, &y, &w, &h);
	eina_rectangle_coords_from(&r, x.final, y.final, w.final,
			h.final);
	/* get the largest rectangle that fits on the matrix */
	enesim_matrix_rect_transform(&prv->matrix, &r, &q);
	enesim_quad_coords_get(&q, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
	enesim_quad_rectangle_to(&q, &r);

#ifdef ETK_DEBUG
	printf("[Etk_Image] Setting geometry of size %d %d %d %d\n",
			r.x, r.y, r.w, r.h);
#endif
	ekeko_renderable_geometry_set((Ekeko_Renderable *)i, &r);
}

static void _matrix_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Image *i = (Etk_Image *)o;
	Etk_Image_Private *prv = PRIVATE(i);
	Enesim_Matrix *m;

	m = em->curr->value.pointer_value;
	enesim_matrix_inverse(m, &prv->inverse);
	/* update the geometry */
	_geometry_calc(o, e, data);
}


static void _file_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Image *i = (Etk_Image *)o;
	Etk_Image_Private *prv = PRIVATE(o);

	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	/* call emage to load an image */
	emage_load_async(em->curr->value.string_value, &prv->src.img, _loader_callback, i, NULL);
}

static void _render(Etk_Shape *s, Etk_Engine *func, Etk_Surface *surface, Etk_Context *context)
{
	Etk_Image *i = (Etk_Image *)s;
	Etk_Image_Private *prv = PRIVATE(i);
	Etk_Coord x, y, w, h;
	Eina_Rectangle srect;

	etk_square_coords_get((Etk_Square *)i, &x, &y, &w, &h);
	eina_rectangle_coords_from(&srect, x.final, y.final, w.final, h.final);
#if ETK_IMAGE_DEBUG
	printf("[Etk_Image] Trying to render the image\n");
#endif
	if (!prv->src.loaded)
	{
		/* in case the image hasnt been loaded yet, send a pattern */
	}
	else
	{
		func->shape->image(surface, context, prv->src.img, &srect);
	}
}

static void _ctor(void *instance)
{
	Etk_Image *i;
	Etk_Image_Private *prv;

	i = (Etk_Image*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_image_type_get(), instance);
	i->parent.parent.render = _render;
	enesim_matrix_identity(&prv->matrix);
	enesim_matrix_inverse(&prv->matrix, &prv->inverse);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_IMAGE_FILE_CHANGED, _file_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_IMAGE_MATRIX_CHANGED, _matrix_change, EINA_FALSE, NULL);
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
Property_Id ETK_IMAGE_MATRIX;

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
		ETK_IMAGE_MATRIX = TYPE_PROP_SINGLE_ADD(type, "matrix", ETK_PROPERTY_MATRIX, OFFSET(Etk_Image_Private, matrix));
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

EAPI void etk_image_matrix_set(Etk_Image *i, Enesim_Matrix *m)
{
	Ekeko_Value v;

	etk_value_matrix_from(&v, m);
	ekeko_object_property_value_set((Ekeko_Object *)i, "matrix", &v);
}
