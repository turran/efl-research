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
	Enesim_Matrix matrix;
	Enesim_Matrix inverse;
	/* TODO add l, t, r, b */
};

static void _loader_callback(Enesim_Surface *s, void *data, int error)
{
	Eon_Canvas *c;
	Eon_Image *i = (Eon_Image *)data;
	Eon_Image_Private *prv = PRIVATE(i);
	Eina_Rectangle r;

	/* TODO check for error */
	if (error)
	{
		printf("[Eon_Image] Error %d %s\n", error, eina_error_msg_get(error));
	}
	else
	{
		c = eon_shape_canvas_get((Eon_Shape *)i);
		ekeko_renderable_geometry_get((Ekeko_Renderable *)i, &r);
#if EON_DEBUG
		printf("[Eon_Image] Image file %s loaded and will be displayed in %p %d %d %d %d!!\n", prv->file.curr, c, r.x, r.y, r.w, r.h);
#endif
		/* add a damage to the canvas to force a redraw of the image */
		ekeko_canvas_damage_add((Ekeko_Canvas *)c, &r);
		prv->src.loaded = EINA_TRUE;
	}
}

static void _geometry_calc(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Eon_Image *i = (Eon_Image *)o;
	Eon_Image_Private *prv = PRIVATE(i);
	Eina_Rectangle r;
	Eon_Coord x, y, w, h;
	Enesim_Quad q;
	float x1, y1, x2, y2, x3, y3, x4, y4;

	/* TODO matrix operation should be:
	 * - multiply w,h * matrix
	 * - translate the result to x, y
	 */
	/* TODO check that the matrix is not identity */
	/* compute the final geometry multiplying by the context matrix */
	eon_square_coords_get((Eon_Square *)i, &x, &y, &w, &h);
	eina_rectangle_coords_from(&r, 0, 0, w.final,
			h.final);
	/* get the largest rectangle that fits on the matrix */
	enesim_matrix_rect_transform(&prv->matrix, &r, &q);
	enesim_quad_coords_get(&q, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
	enesim_quad_rectangle_to(&q, &r);
	r.x = x.final;
	r.y = y.final;

#if EON_DEBUG
	printf("[Eon_Image] Setting geometry of size %d %d %d %d\n",
			r.x, r.y, r.w, r.h);
#endif
	ekeko_renderable_geometry_set((Ekeko_Renderable *)i, &r);
}

static void _matrix_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Image *i = (Eon_Image *)o;
	Eon_Image_Private *prv = PRIVATE(i);
	Enesim_Matrix *m;
	Ekeko_Object *parent;

	m = em->curr->value.pointer_value;
	enesim_matrix_inverse(m, &prv->inverse);
	/* update the geometry */
	_geometry_calc(o, e, data);
	/* store the matrix */
 	if ((parent = ekeko_object_parent_get(o)))
	{
#if 0
		Eon_Engine *func;
		Eon_Document *d;

		d = eon_canvas_document_get((Eon_Canvas *)parent);
		func = eon_document_engine_get(d);
		func->context->matrix_set(eon_shape_context_get((Eon_Shape *)o), &prv->inverse);
#endif
	}
}


static void _file_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Image *i = (Eon_Image *)o;
	Eon_Image_Private *prv = PRIVATE(o);

	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	/* call emage to load an image */
	emage_load_async(em->curr->value.string_value, &prv->src.img, _loader_callback, i, NULL);
}

static void _render(Eon_Shape *s, Eon_Engine *eng, Eon_Surface *surface, Eon_Context *context)
{
	Eon_Image *i = (Eon_Image *)s;
	Eon_Image_Private *prv = PRIVATE(i);
	Eon_Coord x, y, w, h;
	Eina_Rectangle srect;

	eon_square_coords_get((Eon_Square *)i, &x, &y, &w, &h);
	eina_rectangle_coords_from(&srect, x.final, y.final, w.final, h.final);
#if EON_IMAGE_DEBUG
	printf("[Eon_Image] Trying to render the image at %d %d %d %d\n", srect.x, srect.y, srect.w, srect.h);
#endif
	if (!prv->src.loaded)
	{
		/* in case the image hasnt been loaded yet, send a pattern */
	}
	else
	{
		/* TODO pass the image size */
		//func->shape->image(surface, context, prv->src.img, &srect);
	}
}

static void _ctor(void *instance)
{
	Eon_Image *i;
	Eon_Image_Private *prv;

	i = (Eon_Image*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_image_type_get(), instance);
	i->parent.parent.render = _render;
	enesim_matrix_identity(&prv->matrix);
	enesim_matrix_inverse(&prv->matrix, &prv->inverse);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_IMAGE_FILE_CHANGED, _file_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, EON_IMAGE_MATRIX_CHANGED, _matrix_change, EINA_FALSE, NULL);
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

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_IMAGE_FILE;
Ekeko_Property_Id EON_IMAGE_MATRIX;

EAPI Ekeko_Type *eon_image_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_IMAGE, sizeof(Eon_Image),
				sizeof(Eon_Image_Private), eon_square_type_get(),
				_ctor, _dtor, _appendable);
		EON_IMAGE_FILE = TYPE_PROP_DOUBLE_ADD(type, "file", PROPERTY_STRING,
				OFFSET(Eon_Image_Private, file.curr), OFFSET(Eon_Image_Private, file.prev),
				OFFSET(Eon_Image_Private, file.changed));
		EON_IMAGE_MATRIX = TYPE_PROP_SINGLE_ADD(type, "matrix", EON_PROPERTY_MATRIX, OFFSET(Eon_Image_Private, matrix));
	}

	return type;
}

EAPI Eon_Image * eon_image_new(Eon_Canvas *c)
{
	Eon_Image *r;

	r = ekeko_type_instance_new(eon_image_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)r);

	return r;
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

EAPI void eon_image_matrix_set(Eon_Image *i, Enesim_Matrix *m)
{
	Ekeko_Value v;

	eon_value_matrix_from(&v, m);
	ekeko_object_property_value_set((Ekeko_Object *)i, "matrix", &v);
}
