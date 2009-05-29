/*
 * eon_filter.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
#define EON_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Filter_Private *)((Eon_Filter *)(d))->private)
struct _Eon_Filter_Private
{
	Eina_List *shapes;
};

static void _ctor(void *instance)
{
	Eon_Filter *i;
	Eon_Filter_Private *prv;

	i = (Eon_Filter*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_filter_type_get(), instance);
}

static void _dtor(void *filter)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, EON_TYPE_FILTER_EFFECT))
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
EAPI Ekeko_Type *eon_filter_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_FILTER, sizeof(Eon_Filter),
				sizeof(Eon_Filter_Private), ekeko_object_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Eon_Filter * eon_filter_new(Eon_Canvas *c)
{
	Eon_Filter *f;

	f = ekeko_type_instance_new(eon_filter_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)f);

	return f;
}
