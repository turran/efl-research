/*
 * etk2_container.c
 *
 *  Created on: 10-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(cont) ((Container_Private*)(cont->private))

#define TYPE_NAME "Container"

struct _Container_Private
{
	EINA_INLIST;
	Eina_Inlist *children;
};

static void _ctor(void *instance)
{
	Container *cont;
	Container_Private *prv;

	cont = (Container*) instance;
	cont->private = prv = type_instance_private_get(container_type_get(), instance);
	prv->children = NULL;
	printf("[cnt] ctor %p %p\n", cont, cont->private);
}

static void _dtor(void *container)
{
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *container_type_get(void)
{
	static Type *container_type = NULL;

	if (!container_type)
	{
		container_type = type_new(TYPE_NAME, sizeof(Container), sizeof(Container_Private), object_type_get(), _ctor, _dtor, NULL, NULL);
	}

	return container_type;
}
/**
 *
 * @param p
 * @param o
 */
EAPI void container_child_append(Container *p, Container *o)
{
	if (!p->appendable)
		return;
	/* append the child */
	if (p->appendable(object_type_name_get((Object *)o)))
	{
		Container_Private *pprv, *oprv;
		Event_Mutation evt;

		pprv = PRIVATE(p);
		oprv = PRIVATE(o);
		pprv->children = eina_inlist_append(pprv->children, EINA_INLIST_GET(oprv));
		/* send the event */
		event_mutation_init(&evt, EVENT_OBJECT_APPEND, (Object *)o, (Object *)p, NULL, NULL, NULL);
		object_event_dispatch((Object *)p, (Event *)&evt);
	}
}

/**
 *
 * @param p
 * @param o
 */
EAPI void container_child_remove(Container *p, Container *o)
{

}
