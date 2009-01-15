/*
 * dummy_canvas.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Canvas_Private*)(d->private))

#define DUMMY_CANVAS_TYPE_NAME "DummyCanvas"


struct _Dummy_Canvas_Private
{

};

static void _flush(Canvas *c, Eina_Rectangle *r)
{

}

static void _ctor(void *instance)
{
	Dummy_Canvas *dc;
	Dummy_Canvas_Private *prv;

	dc = (Dummy_Canvas*) instance;
	dc->private = prv = type_instance_private_get(dummy_canvas_type_get(), instance);
	dc->parent.flush = _flush;
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type *dummy_canvas_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_CANVAS_TYPE_NAME, sizeof(Dummy_Canvas),
				sizeof(Dummy_Canvas_Private), canvas_type_get(), _ctor, _dtor);
	}

	return type;
}

