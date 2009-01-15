/*
 * dummy.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Private*)(d->private))

#define DUMMY_TYPE_NAME "Dummy"

struct _Dummy_Private
{
	Dummy_Canvas *dc;
};

static void _ctor(void *instance)
{
	Dummy *dc;
	Dummy_Private *prv;

	dc = (Dummy*) instance;
	dc->private = prv = type_instance_private_get(dummy_type_get(), instance);
	/* initialize SDL */
	/* TODO create the canvas */
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type *dummy_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_TYPE_NAME, sizeof(Dummy),
				sizeof(Dummy_Private), object_type_get(), _ctor, _dtor);
	}

	return type;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
void dummy_init(void)
{
	main_init();
}

void dummy_shutdown(void)
{
	main_shutdown();
}

Dummy * dummy_new(void)
{
	return type_instance_new(dummy_type_get());
}

void dummy_loop(Dummy *d)
{

}

Dummy_Canvas * dummy_canvas_get(Dummy *d)
{
	Dummy_Private *prv;

	prv = PRIVATE(d);
	return prv->dc;
}
