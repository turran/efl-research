/*
 * dummy_rect.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Dummy_Rect_Private*)(d->private))

#define DUMMY_RECT_TYPE_NAME "DummyRect"


struct _Dummy_Rect_Private
{

};

static void _ctor(void *instance)
{
	Dummy_Rect *dc;
	Dummy_Rect_Private *prv;

	dc = (Dummy_Rect*) instance;
	dc->private = prv = type_instance_private_get(dummy_rect_type_get(), instance);
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type * dummy_rect_type_get(void)
{
	static Type *type = NULL;

	if (!type)
	{
		type = type_new(DUMMY_RECT_TYPE_NAME, sizeof(Dummy_Rect),
				sizeof(Dummy_Rect_Private), renderable_type_get(), _ctor, _dtor);
	}

	return type;
}
Dummy_Rect * dummy_rect_new(void)
{
	return type_instance_new(dummy_rect_type_get());
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Dummy_Rect * dummy_rect_add(Dummy_Canvas *dc)
{
	Dummy_Rect *dr;

	dr = dummy_rect_new();
	/* attach the rect to the canvas */
	object_child_append((Object *)dc, (Object *)dr);
	return dr;
}
