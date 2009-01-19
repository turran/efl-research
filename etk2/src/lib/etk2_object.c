#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(obj) ((Object_Private*)(obj->private))

#define TYPE_NAME "Object"

struct _Object_Private
{
	char *id;
	Type *type;
	Eina_Hash *listeners;
	Eina_Hash *user;
	EINA_INLIST;
	Eina_Inlist *children;
	Object *parent;
	/* we need a changed counter, to keep track of every async prop change */
	int changed;
};

/* We use a container for the object-event in case we need
 * to store more info
 */
typedef struct _Object_Event
{
	Event_Listener el;
	Eina_Bool bubble;
} Object_Event;

static void _id_modify(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;

	/* TODO we should avoid that many strcmp() */
	//if (!strcmp(em->prop, "id"))
	if (em->prop_id == OBJECT_ID_ID)
	{
		/* Send the idChanged event */
	}
}

static void _ctor(void *instance)
{
	Object *obj;
	Object_Private *prv;

	obj = (Object*) instance;
	obj->private = prv = type_instance_private_get(object_type_get(), instance);
	prv->listeners = eina_hash_string_superfast_new(NULL);
	prv->user = eina_hash_string_superfast_new(NULL);
	prv->children = NULL;
	prv->parent = NULL;
	/* Set up the mutation event */
	object_event_listener_add(obj, EVENT_PROP_MODIFY, _id_modify, EINA_FALSE);
	printf("[obj] ctor %p %p %p\n", obj, obj->private, prv->type);
}

static void _dtor(void *object)
{
	printf("[obj] dtor %p\n", object);
}

static void _event_dispatch(Object *obj, Event *e, Eina_Bool bubble)
{
	Eina_List *listeners;
	Eina_Iterator *it;
	Object_Event *oe;
	Object_Private *prv;

	prv = PRIVATE(obj);
	listeners = eina_hash_find(prv->listeners, e->type);
	if (!listeners) return;

	it = eina_list_iterator_new(listeners);
	while (eina_iterator_next(it, (void **)&oe))
	{
		if (oe->bubble == bubble)
			oe->el(obj, e);
	}
}

/* FIXME where should this be? */
static Eina_Bool _property_changed(Object *obj)
{
    Eina_Bool changed_bef, changed_now;
	Object_Private *prv;
	prv = PRIVATE(obj);

	/* get the property */
	/* get the changed */
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Type * object_private_type_get(Object *object)
{
	return object->private->type;
}

void object_construct(Type *type, void *instance)
{
	Object *object;

	object = (Object*) instance;
	object->private = type_instance_private_get_internal(type, object_type_get(), object);
	object->private->type = type;
	printf("[obj] construct %p %p %p\n", object, object->private, object->private->type);
	/* call all the constructors on the type */
	type_construct(type, instance);
}

void object_event_listener_add(Object *obj, const char *type,
		Event_Listener el, Eina_Bool bubble)
{
	Object_Private *prv;
	Object_Event *oe;
	Eina_List *events, *lst;

	oe = malloc(sizeof(Object_Event));
	oe->el = el;
	oe->bubble = bubble;

	prv = PRIVATE(obj);
	events = eina_hash_find(prv->listeners, type);
	lst = eina_list_append(events, oe);
	if (!events)
		eina_hash_add(prv->listeners, type, lst);
}

void object_event_listener_remove(Object *obj, const char *type,
		Event_Listener el)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Property_Id OBJECT_ID_ID;

Type *object_type_get(void)
{
	static Type *object_type = NULL;

	if (!object_type)
	{
		object_type = type_new(TYPE_NAME, sizeof(Object),
				sizeof(Object_Private), NULL, _ctor, _dtor);
		OBJECT_ID_ID = TYPE_PROP_SINGLE_ADD(object_type, "id", PROPERTY_STRING, OFFSET(Object_Private, id));
		// TODO register the type's event, with type_event_new
	}

	return object_type;
}
/**
 *
 * @return
 */
EAPI Object *object_new(void)
{
	Object *object;

	object = type_instance_new(object_type_get());

	return object;
}
/**
 *
 * @param object
 * @param name
 */
EAPI void object_id_set(Object *object, const char *name)
{
	Value value;

	value_str_from(&value, (char *)name);
	object_property_value_set(object, "id", &value);
}
/**
 *
 * @param object
 * @return
 */
EAPI const char *object_id_get(Object *object)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	return prv->id;
}
/**
 *
 * @param object
 * @param prop_name
 * @param value
 */
EAPI void object_property_value_set(Object *object, char *prop_name, Value *value)
{
	Object_Private *prv;
	Property *prop;
	Eina_List *listeners;
	RETURN_IF(object == NULL || prop_name == NULL);

	prv = PRIVATE(object);
	printf("[obj] value_set: %s %p %p %p\n", prop_name, object, prv, prv->type);
	/* FIXME this code isnt good enough */
	prop = type_property_get(prv->type, prop_name);
	if (!prop)
		return;
	listeners = eina_hash_find(prv->listeners, EVENT_PROP_MODIFY);
	if (listeners)
	{
		Event_Mutation evt;
		Value prev;

		if (!type_instance_property_value_set(prv->type, object, prop_name, value, &prev))
			return;
		event_mutation_init(&evt, EVENT_PROP_MODIFY, object, object, prop, &prev, value);
		_event_dispatch(object, (Event *)&evt, EINA_FALSE);
		/* TODO a property change might have an allocated string, free it */
	}
	else
	{
		type_instance_property_value_set(prv->type, object, prop_name, value, NULL);
	}
}
/**
 *
 * @param object
 * @param prop_name
 * @param value
 */
EAPI void object_property_value_get(Object *object, char *prop_name, Value *value)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	printf("[obj] value_get: %s\n", prop_name);
	type_instance_property_value_get(prv->type, object, prop_name, value);
}
/**
 *
 * @param object
 * @param name
 * @param data
 */
EAPI void object_user_data_set(Object *object, const char *name, void *data)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	eina_hash_add(prv->user, name, data);
}
/**
 *
 * @param object
 * @param name
 */
EAPI void * object_user_data_get(Object *object, const char *name)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	return eina_hash_find(prv->user, name);
}

EAPI void object_event_dispatch(const Object *obj, Event *e)
{
	Object_Private *prv;

	/* TODO set the phase on the event */
	prv = PRIVATE(obj);
	_event_dispatch(obj, e, EINA_FALSE);
	if (e->bubbles == EINA_TRUE)
	{
		printf("Event %s going to bubble %p %p\n", e->type, obj, prv->parent);
		while (prv->parent)
		{
			Object *parent = prv->parent;
			prv = PRIVATE(parent);
			printf("dispatching event\n");
			_event_dispatch(parent, e, EINA_TRUE);
		}
	}
}

EAPI const char * object_type_name_get(const Object *obj)
{
	Object_Private *prv;

	prv = PRIVATE(obj);
	return type_name_get(prv->type);
}

/**
 *
 * @param p
 * @param o
 */
EAPI void object_child_append(Object *p, Object *o)
{
	if (!p->appendable)
		return;
	/* append the child */
	if (p->appendable(object_type_name_get((Object *)o)))
	{
		Object_Private *pprv, *oprv;
		Event_Mutation evt;

		printf("Setting the parent of %p to %p\n", o, p);
		pprv = PRIVATE(p);
		oprv = PRIVATE(o);
		pprv->children = eina_inlist_append(pprv->children, EINA_INLIST_GET(oprv));
		/* TODO check that there's a parent already */
		oprv->parent = p;
		/* send the event */
		event_mutation_init(&evt, EVENT_OBJECT_APPEND, (Object *)o, (Object *)p, NULL, NULL, NULL);
		object_event_dispatch((Object *)o, (Event *)&evt);
	}
}

/**
 *
 * @param p
 * @param o
 */
EAPI void object_child_remove(Object *p, Object *o)
{

}

EAPI Object * object_parent_get(Object *o)
{
	Object_Private *prv;

	prv = PRIVATE(o);
	return prv->parent;
}

EAPI void object_process(Object *o)
{
	/* TODO check that the object is actually the top most parent
	 * on the hierarchy?
	 */
	/* iterate over the childs, like the ekeko algo etc */
#if 0
    Ekeko_Node_Private *np;
	Eina_Iterator *it;
	Ekeko_Node *in;
	Ekeko_Event_Process *e = NULL;

	np = n->p;
	/* all childs */
	if (!np->changed)
		return;
	//printf("[0  Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* handle the attributes as they dont have any parent, childs or siblings */
	if (np->type == EKEKO_NODE_ELEMENT)
	{
		_element_process(np);
		if (!np->changed)
			return;
	}
	/* handle childs */
	it = eina_inlist_iterator_new(np->childs);
	while (eina_iterator_next(it, (void **) &in))
	{
		int changed = in->changed;

		if (!changed)
			continue;

		ekeko_node_process(in);
		np->changed -= changed;
		if (!np->changed)
		{
			break;
		}
	}
	//printf(" 0] Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* post condition */
	assert(!np->changed);
	if (np->type == EKEKO_NODE_DOCUMENT)
		e = (Ekeko_Event_Process *) ekeko_document_event_new(
				(Ekeko_Document *) np, "ProcessEvents");
	else if (np->type == EKEKO_NODE_ELEMENT)
		e = (Ekeko_Event_Process *) ekeko_document_event_new(np->owner,
				"ProcessEvents");
	else
		return;
	ekeko_event_process_init(e, np, EKEKO_EVENT_PROCESS_PHASE_POST);
	ekeko_node_event_dispatch(np, (Ekeko_Event *) e);
#endif
}
