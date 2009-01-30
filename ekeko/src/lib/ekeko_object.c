#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(obj) ((Ekeko_Object_Private*)(obj->private))

#define TYPE_NAME "Object"

struct _Ekeko_Object_Private
{
	EINA_INLIST;
	char *id;
	Ekeko_Type *type;
	Eina_Hash *listeners;
	Eina_Hash *user;
	Eina_Inlist *children;
	Ekeko_Object *parent;
	Ekeko_Object *rel; /* when we store an inlist we only store the private no the public memory area */
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

static void _id_modify(const Ekeko_Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;

	/* TODO we should avoid that many strcmp() */
	//if (!strcmp(em->prop, "id"))
	if (em->prop_id == EKEKO_OBJECT_PROPERTY_ID)
	{
		/* Send the idChanged event */
	}
}

static void _ctor(void *instance)
{
	Ekeko_Object *obj;
	Ekeko_Object_Private *prv;

	obj = (Ekeko_Object*) instance;
	obj->private = prv = ekeko_type_instance_private_get(ekeko_object_type_get(), instance);
	prv->listeners = eina_hash_string_superfast_new(NULL);
	prv->user = eina_hash_string_superfast_new(NULL);
	prv->children = NULL;
	prv->parent = NULL;
	prv->changed = 0;
	prv->rel = obj;
	/* Set up the mutation event */
	object_event_listener_add(obj, EVENT_PROP_MODIFY, _id_modify, EINA_FALSE);
#ifdef ETK2_DEBUG
	printf("[obj] ctor %p %p %p\n", obj, obj->private, prv->type);
#endif
}

static void _dtor(void *object)
{
#ifdef ETK2_DEBUG
	printf("[obj] dtor %p\n", object);
#endif
}

static void _event_dispatch(const Ekeko_Object *obj, Event *e, Eina_Bool bubble)
{
	Eina_List *listeners;
	Eina_Iterator *it;
	Object_Event *oe;
	Ekeko_Object_Private *prv;

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

static void _change_recursive(const Ekeko_Object *obj, int count)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(obj);
	prv->changed += count;
	if (prv->parent)
			_change_recursive(prv->parent, count);
}

static void _unchange_recursive(const Ekeko_Object *obj, int count)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(obj);
	prv->changed -= count;
	if (prv->parent)
		_unchange_recursive(prv->parent, count);

}

static void _pointer_single_value_set(void *ptr, Value_Type type, Value *value)
{
	switch (type)
	{
		case PROPERTY_INT:
		*((int *)ptr) = value->value.int_value;
		break;

		case PROPERTY_STRING:
		*((char **)ptr) = strdup(value->value.string_value);
		break;

		case PROPERTY_RECTANGLE:
		*((Eina_Rectangle *)ptr) = value->value.rect;
		break;

		case PROPERTY_BOOL:
		*((Eina_Bool *)ptr) = value->value.bool_value;
		break;

		default:
		printf("POINTER SINGLE VALUE SET UNDEFINED VALUE\n");
		break;
	}
}

static void _pointer_double_value_set(void *ptr, void *prev, char *changed,
		Value_Type type, Value *value)
{
	switch (type)
	{
		case PROPERTY_INT:
		*((int *)ptr) = value->value.int_value;
		if (*((int *)ptr) != *((int *)prev))
			*changed = EINA_TRUE;
		break;

		case PROPERTY_STRING:
		*((char **)ptr) = strdup(value->value.string_value);
		if (!strcmp(*((char **)ptr), *((char **)prev)))
			*changed = EINA_TRUE;
		break;

		case PROPERTY_RECTANGLE:

		{
			Eina_Rectangle *c = (Eina_Rectangle *)ptr;
			Eina_Rectangle *p;

			*c = value->value.rect;
			p = (Eina_Rectangle *)prev;
			if ((c->x != p->x) || (c->y != p->y) || (c->w != p->w) || (c->h != p->h))
				*changed = EINA_TRUE;
		}
		break;

		case PROPERTY_BOOL:
		*((Eina_Bool *)ptr) = value->value.bool_value;
		if (*((Eina_Bool *)ptr) != *((Eina_Bool *)prev))
			*changed = EINA_TRUE;
		break;

		default:
		printf("POINTER DOUBLE VALUE SET UNDEFINED VALUE\n");
		break;
	}
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Type * object_private_type_get(Ekeko_Object *object)
{
	return object->private->type;
}

void object_construct(Ekeko_Type *type, void *instance)
{
	Ekeko_Object *object;

	object = (Ekeko_Object*) instance;
	object->private = type_instance_private_get_internal(type, ekeko_object_type_get(), object);
	object->private->type = type;
#ifdef ETK2_DEBUG
	printf("[obj] construct %p %p %p\n", object, object->private, object->private->type);
#endif
	/* call all the constructors on the type */
	type_construct(type, instance);
}

void object_event_listener_add(Ekeko_Object *obj, const char *type,
		Event_Listener el, Eina_Bool bubble)
{
	Ekeko_Object_Private *prv;
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

void object_event_listener_remove(Ekeko_Object *obj, const char *type,
		Event_Listener el)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Property_Id EKEKO_OBJECT_PROPERTY_ID;

Ekeko_Type *ekeko_object_type_get(void)
{
	static Ekeko_Type *object_type = NULL;

	if (!object_type)
	{
		object_type = ekeko_type_new(TYPE_NAME, sizeof(Ekeko_Object),
				sizeof(Ekeko_Object_Private), NULL, _ctor, _dtor, NULL);
		EKEKO_OBJECT_PROPERTY_ID = TYPE_PROP_SINGLE_ADD(object_type, "id", PROPERTY_STRING, OFFSET(Ekeko_Object_Private, id));
		// TODO register the type's event, with type_event_new
	}

	return object_type;
}
/**
 *
 * @return
 */
EAPI Ekeko_Object *ekeko_object_new(void)
{
	Ekeko_Object *object;

	object = ekeko_type_instance_new(ekeko_object_type_get());

	return object;
}
/**
 *
 * @param object
 * @param name
 */
EAPI void ekeko_object_id_set(Ekeko_Object *object, const char *name)
{
	Value value;

	value_str_from(&value, (char *)name);
	ekeko_object_property_value_set(object, "id", &value);
}
/**
 *
 * @param object
 * @return
 */
EAPI const char *ekeko_object_id_get(Ekeko_Object *object)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(object);
	return prv->id;
}
/**
 *
 * @param object
 * @param prop_name
 * @param value
 */
EAPI void ekeko_object_property_value_set(Ekeko_Object *object, char *prop_name, Value *value)
{
	Ekeko_Object_Private *prv;
	Property *prop;
	Value prev_value;
	Event_Mutation evt;

	void *curr, *prev;
	char *changed;

	RETURN_IF(object == NULL || prop_name == NULL);

	prv = PRIVATE(object);
#ifdef ETK2_DEBUG
	printf("[obj] value_set: %s %p %p %p %d\n", prop_name, object, prv, prv->type, prv->changed);
#endif
	/* FIXME this code isnt good enough */
	prop = type_property_get(prv->type, prop_name);
	if (!prop)
		return;
	type_instance_property_pointers_get(prv->type, prop, object, &curr, &prev, &changed);
#ifdef ETK2_DEBUG
	printf("[obj] pointers %p %p %p\n", curr, prev, changed);
#endif
	if (property_ptype_get(prop) == PROPERTY_VALUE_DUAL_STATE)
	{
		Eina_Bool changed_bef, changed_now;

		changed_bef = *changed;
		value_set(&prev_value, ekeko_property_value_type_get(prop), prev);
		_pointer_double_value_set(curr, prev, changed, ekeko_property_value_type_get(prop), value);
		changed_now = *changed;
		if (changed_bef && !changed_now)
		{
			_unchange_recursive(object, 1);
		}
		else if (!changed_bef && changed_now)
		{
			_change_recursive(object, 1);
		}
#ifdef ETK2_DEBUG
		printf("[obj] changed bef = %d, changed now = %d\n", changed_bef, changed_now);
#endif
	}
	else
	{
		value_set(&prev_value, ekeko_property_value_type_get(prop), curr);
		_pointer_single_value_set(curr, ekeko_property_value_type_get(prop), value);
	}
#ifdef ETK2_DEBUG
	printf("[obj] changed = %d\n", prv->changed);
#endif
	/* send the event */
	event_mutation_init(&evt, EVENT_PROP_MODIFY, object, object, prop,
			&prev_value, value, EVENT_MUTATION_STATE_CURR);
	ekeko_object_event_dispatch((Ekeko_Object *)object, (Event *)&evt);
}
/**
 *
 * @param object
 * @param prop_name
 * @param value
 */
EAPI void ekeko_object_property_value_get(Ekeko_Object *object, char *prop_name, Value *value)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(object);
#ifdef ETK2_DEBUG
	printf("[obj] value_get: %s\n", prop_name);
#endif
	type_instance_property_value_get(prv->type, object, prop_name, value);
}
/**
 *
 * @param object
 * @param name
 * @param data
 */
EAPI void ekeko_object_user_data_set(Ekeko_Object *object, const char *name, void *data)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(object);
	eina_hash_add(prv->user, name, data);
}
/**
 *
 * @param object
 * @param name
 */
EAPI void * ekeko_object_user_data_get(Ekeko_Object *object, const char *name)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(object);
	return eina_hash_find(prv->user, name);
}

EAPI void ekeko_object_event_dispatch(const Ekeko_Object *obj, Event *e)
{
	Ekeko_Object_Private *prv;

	/* TODO set the phase on the event */
	prv = PRIVATE(obj);
#ifdef ETK2_DEBUG
	printf("[obj] Dispatching event %s\n", e->type);
#endif
	_event_dispatch(obj, e, EINA_FALSE);
	if (e->bubbles == EINA_TRUE)
	{
#ifdef ETK2_DEBUG
		printf("[obj] Event %s going to bubble %p %p\n", e->type, obj, prv->parent);
#endif
		while (prv->parent)
		{
			Ekeko_Object *parent = prv->parent;
			prv = PRIVATE(parent);
			_event_dispatch(parent, e, EINA_TRUE);
		}
	}
}

EAPI const char * ekeko_object_type_name_get(const Ekeko_Object *obj)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(obj);
	return type_name_get(prv->type);
}

/**
 *
 * @param p
 * @param o
 */
EAPI void ekeko_object_child_append(Ekeko_Object *p, Ekeko_Object *o)
{
	Ekeko_Object_Private *prv;
	Ekeko_Type *t;

	prv = PRIVATE(p);
	t = prv->type;
	if (type_appendable(t, ekeko_object_type_name_get((Ekeko_Object *)o)))
	{
		Ekeko_Object_Private *pprv, *oprv;
		Event_Mutation evt;

		pprv = PRIVATE(p);
		oprv = PRIVATE(o);
#ifdef ETK2_DEBUG
		printf("[obj] Setting the parent of %p (%p) to %p (%p) \n", o, oprv, p, pprv);
#endif
		pprv->children = eina_inlist_append(pprv->children, EINA_INLIST_GET(oprv));
		/* TODO check that there's a parent already
		 * if so, send an event informing that the child has been removed
		 * from that parent
		 */
		/* check if the object has some pending changes */
		if (oprv->changed)
		{
			if (oprv->parent)
				_unchange_recursive(oprv->parent, oprv->changed);
			_change_recursive(p, oprv->changed);
		}
		oprv->parent = p;
#ifdef ETK2_DEBUG
		printf("[obj] pchanged = %d ochanged = %d\n", pprv->changed, oprv->changed);
#endif
		/* send the event */
		event_mutation_init(&evt, EVENT_OBJECT_APPEND, (Ekeko_Object *)o, (Ekeko_Object *)p, NULL, NULL, NULL,
				EVENT_MUTATION_STATE_CURR);
		ekeko_object_event_dispatch((Ekeko_Object *)o, (Event *)&evt);
	}
}

/**
 *
 * @param p
 * @param o
 */
EAPI void ekeko_object_child_remove(Ekeko_Object *p, Ekeko_Object *o)
{

}

EAPI Ekeko_Object * ekeko_object_parent_get(Ekeko_Object *o)
{
	Ekeko_Object_Private *prv;

	prv = PRIVATE(o);
	return prv->parent;
}

/**
 * @brief This function will update every two state attribute in case it has
 * changed
 * @param o
 */
EAPI void ekeko_object_process(Ekeko_Object *o)
{
	Ekeko_Object_Private *prv;
	Ekeko_Object_Private *in;
	Eina_Iterator *it;
	Property_Iterator *pit;
	Property *prop;
	Event_Mutation evt;

	/* TODO check that the object is actually the top most parent
	 * on the hierarchy?
	 */
	prv = PRIVATE(o);
	/* all childs */
	if (!prv->changed)
		return;
#ifdef ETK2_DEBUG
	printf("[obj] [0  Object %p %s changed %d\n", o, ekeko_object_type_name_get(o), prv->changed);
#endif
	/* TODO handle the attributes as they dont have any parent, childs or siblings */
	pit = type_property_iterator_new(prv->type);
	while (type_property_iterator_next(pit, &prop))
	{
		Value prev_value, curr_value;
		Event_Mutation evt;
		void *curr, *prev;
		char *changed;

		if (property_ptype_get(prop) != PROPERTY_VALUE_DUAL_STATE)
			continue;
		type_instance_property_pointers_get(prv->type, prop, o, &curr, &prev, &changed);
#ifdef ETK2_DEBUG
		printf("[obj] process pointers %p %p %p\n", curr, prev, changed);
#endif
		if (!(*changed))
			continue;
#ifdef ETK2_DEBUG
		printf("[obj] [1 updating %s %d\n", ekeko_property_name_get(prop), *changed);
#endif

		/* update the property */
		*changed = EINA_FALSE;
		prv->changed--;
		/* send the mutation event */
		value_set(&prev_value, ekeko_property_value_type_get(prop), prev);
		value_set(&curr_value, ekeko_property_value_type_get(prop), curr);
		event_mutation_init(&evt, EVENT_PROP_MODIFY, o, o, prop, &prev_value,
				&curr_value, EVENT_MUTATION_STATE_POST);
		ekeko_object_event_dispatch(o, (Event *)&evt);
		/* update prev */
		_pointer_single_value_set(prev, ekeko_property_value_type_get(prop), &curr_value);
		if (!prv->changed)
		{
			type_property_iterator_free(pit);
#ifdef ETK2_DEBUG
			printf("[obj] 0] Object changed %d (only attributes)\n", prv->changed);
#endif
			goto event;
		}
	}
	/* handle childs */
	it = eina_inlist_iterator_new(prv->children);
	while (eina_iterator_next(it, (void **) &in))
	{
		int changed;

		changed = in->changed;

		if (!changed)
			continue;

		ekeko_object_process(in->rel);
		prv->changed -= changed;
		if (!prv->changed)
		{
			break;
		}
	}
#ifdef ETK2_DEBUG
	printf("[obj] 0] Object changed %d\n", prv->changed);
#endif
	/* post condition */
	if (prv->changed)
	{
		printf("WRONG! %p %d\n", o, prv->changed);
		exit(1);
	}
event:
	/* send the event */
	event_mutation_init(&evt, EVENT_OBJECT_PROCESS, o, NULL, NULL, NULL, NULL,
			EVENT_MUTATION_STATE_POST);
	ekeko_object_event_dispatch(o, (Event *)&evt);
}

EAPI Property * ekeko_object_property_get(Ekeko_Object *o, const char *prop_name)
{
	Ekeko_Object_Private *prv;
	Property *prop;

	prv = PRIVATE(o);
	prop = type_property_get(prv->type, prop_name);
	return prop;
}
