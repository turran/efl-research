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
	/* create the canvas */
	prv->dc = dummy_canvas_new();
	/* TODO mark the canvas as the top most */
}

static void _dtor(void *canvas)
{

}

static Dummy *_dummy = NULL;

static int _idler(void *data)
{
	Dummy_Private *prv;
	if (!_dummy)
		return 1;

	/* process the objects!! :) */
	object_process((Object *)_dummy->private->dc);
	return 1;
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
	/* initialize etk2 */
	main_init();
	ecore_init();
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);
	/* add the idler */
	ecore_idle_enterer_add(_idler, NULL);
	//ecore_idler_add(_idler, NULL);
}

void dummy_shutdown(void)
{
	ecore_main_loop_quit();
	main_shutdown();
	ecore_shutdown();
}

Dummy * dummy_new(void)
{
	/* Singleton for now */
	if (!_dummy)
		_dummy = type_instance_new(dummy_type_get());
	return _dummy;
}

void dummy_loop(void)
{
	printf("loop\n");
	ecore_main_loop_begin();
	printf("quit\n");
}

Dummy_Canvas * dummy_canvas_get(Dummy *d)
{
	Dummy_Private *prv;

	prv = PRIVATE(d);
	return prv->dc;
}
