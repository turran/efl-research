/*
 * etk2_filter.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#define ETK_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Filter_Private *)((Etk_Filter *)(d))->private)
struct _Etk_Filter_Private
{
	Eina_List *shapes;
};

static void _ctor(void *instance)
{
	Etk_Filter *i;
	Etk_Filter_Private *prv;

	i = (Etk_Filter*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_filter_type_get(), instance);
}

static void _dtor(void *filter)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, ETK_TYPE_FILTER_EFFECT))
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
EAPI Ekeko_Type *etk_filter_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_FILTER, sizeof(Etk_Filter),
				sizeof(Etk_Filter_Private), ekeko_object_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Etk_Filter * etk_filter_new(Etk_Canvas *c)
{
	Etk_Filter *f;

	f = ekeko_type_instance_new(etk_filter_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)f);

	return f;
}
