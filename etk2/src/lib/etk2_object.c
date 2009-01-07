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
	char *name;
	Type *type;
	Eina_Hash *listeners;
};

/* We use a container for the object-event in case we need
 * to store more info
 */
typedef struct _Object_Event
{
	Event_Listener el;
} Object_Event;

static void object_ctor(void *instance)
{
	Object *obj;
	Object_Private *prv;

	obj = (Object*) instance;
	prv = PRIVATE(obj);
	prv->listeners = eina_hash_string_superfast_new(NULL);

	/* TODO create a hash for user provided data */
	printf("[obj] ctor %p %p %p\n", obj, obj->private, prv->type);
}

static void object_dtor(void *object)
{
	printf("[obj] dtor %p\n", object);
}

static void _event_dispatch(Eina_List *listeners, Event *e)
{
	Eina_Iterator *it;
	Object_Event *oe;

	it = eina_list_iterator_new(listeners);
	while (eina_iterator_next(it, (void **)&oe))
	{
               oe->el(e);
	}
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
		Event_Listener el)
{
	Object_Private *prv;
	Object_Event *oe;
	Eina_List *events, *lst;

	oe = malloc(sizeof(Object_Event));
	oe->el = el;

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
Type *object_type_get(void)
{
	static Type *object_type = NULL;

	if (!object_type)
	{
		object_type = type_new(TYPE_NAME, sizeof(Object), sizeof(Object_Private), NULL, object_ctor, object_dtor, object_property_value_set, object_property_value_get);
		type_property_new(object_type, "name", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Object_Private, name), NULL);
	}

	return object_type;
}

Object *object_new(void)
{
	Object *object;

	object = type_instance_new(object_type_get());

	return object;
}

void object_name_set(Object *object, const char *name)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	if (name)
		prv->name = strdup(name);
}

const char *object_name_get(Object *object)
{
	Object_Private *prv;

	prv = PRIVATE(object);
	return prv->name;
}

void object_property_value_set(Object *object, char *prop_name, Type_Property_Value *value)
{
	Object_Private *prv;
	Eina_List *listeners;
	RETURN_IF(object == NULL || prop_name == NULL);

	prv = PRIVATE(object);
	printf("[obj] value_set: %s %p %p %p\n", prop_name, object, prv, prv->type);
	listeners = eina_hash_find(prv->listeners, "PropModified");
	if (listeners)
	{
		Event_Mutation *evt;

		/* TODO do we actually need to allocate a property? */
		evt = malloc(sizeof(Event_Mutation));
		evt->event.type = "PropModified";
		evt->related = evt->event.target = object;
		type_instance_property_value_get(prv->type, object, prop_name, &evt->prev);
		type_instance_property_value_set(prv->type, object, prop_name, value);
		evt->curr = *value;
		_event_dispatch(listeners, (Event *)evt);
		/* TODO free the event */
	}
	else
	{
		type_instance_property_value_set(prv->type, object, prop_name, value);
	}
}

void object_property_value_get(Object *object, char *prop_name, Type_Property_Value *value)
{
	printf("[obj] value_get: %s\n", prop_name);
	type_instance_property_value_get(object->private->type, object, prop_name, value);
}
