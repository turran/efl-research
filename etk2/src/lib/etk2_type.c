#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/**
 * @brief
 */
struct _Type
{
	char *name;
	size_t size;
	size_t priv_size;
	Type *parent;

	Type_Constructor ctor;
	Type_Destructor dtor;

	Eina_Hash *properties;
};

static size_t type_public_size_get(Type *type)
{
	return type->size;
}

static size_t type_private_size_get(Type *type)
{
	size_t parent_size = 0;

	if (!type) return 0;
	if (type->parent) parent_size = type_private_size_get(type->parent);

	return type->priv_size + parent_size;
}

static size_t type_size_get(Type *type)
{
	size_t parent_size = 0;

	if (!type) return 0;
	if (type->parent) parent_size = type_size_get(type->parent);

	return type->size + type->priv_size + parent_size;
}

static inline void * _instance_property_offset_get(Type *type, Property *prop,
		ssize_t poffset, void *instance)
{
	Type *pt = property_type_get(prop);

	int offset = type_public_size_get(type) + type_private_size_get(pt->parent);
	return (char *)instance + offset + poffset;
}

static inline void * _instance_property_curr_ptr_get(Type *type, Property *prop, void *instance)
{
	ssize_t poffset = property_curr_offset_get(prop);

	return _instance_property_offset_get(type, prop, poffset, instance);
}

static inline void * _instance_property_prev_ptr_get(Type *type, Property *prop, void *instance)
{
	ssize_t poffset = property_prev_offset_get(prop);

	return _instance_property_offset_get(type, prop, poffset, instance);
}

static inline void * _instance_property_changed_ptr_get(Type *type, Property *prop, void *instance)
{
	ssize_t poffset = property_changed_offset_get(prop);

	return _instance_property_offset_get(type, prop, poffset, instance);
}

static void type_destruct_internal(Type *type, void *object)
{
	if (!type)
		return;

	if (type->dtor)
		type->dtor(object);

	if (type->parent)
		type_destruct_internal(type->parent, object);
}

/**
 *
 * @param type
 * @param prop_name
 * @return
 */
static Property *_property_get(Type *type, const char *prop_name)
{
	Property *property = NULL;

	do
	{
		property = eina_hash_find(type->properties, prop_name);
		if (!property)
			type = type->parent;
	} while (!property && type);

	return property;
}

static inline void _property_string_set(Value *vc, Value *vp, char **c, char **p, char *changed)
{
	char **str = (char**)c;
	if (vp) vp->value.string_value = *str;
	*str = strdup(vc->value.string_value);
	/* TODO cmp curr and prev and update the change flag */
	if (p)
	{

	}
}

static inline void _property_int_set(Value *vc, Value *vp, int *c, int *p, char *changed)
{
	if (vp) vp->value.int_value = *c;
	*c = vc->value.int_value;
	/* TODO cmp curr and prev and update the change flag */
	if (p)
	{

	}
}

/* TODO should we register types per document?
 * Note that type_new_name_from wont work until the that type has been
 * added i.e first calling type_new()
 */
static Eina_Hash *_types = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void type_construct(Type *t, void *instance)
{
	if (!t)
		return;

	if (t->parent)
		type_construct(t->parent, instance);

	if (t->ctor)
		t->ctor(instance);
}

void * type_instance_private_get_internal(Type *final, Type *t, void *instance)
{
	printf("[type] private get %s (PUB=%d) %s (PRIV_OFF=%d) %p\n", final->name, type_public_size_get(final), t->name, type_private_size_get(t->parent), instance);
	return (char *)instance + type_public_size_get(final) + type_private_size_get(t->parent);
}

Property * type_property_get(Type *t, const char *name)
{
	return _property_get(t, name);
}

void type_instance_property_value_get(Type *type, void *instance, char *prop_name, Value *v)
{
	Property *property;
	void *curr;

	if (!type || !instance || !prop_name)
		return;
	property = _property_get(type, prop_name);
	if (!property)
		return;
	curr = _instance_property_curr_ptr_get(type, property, instance);
	value_set(v, property_value_type_get(property), curr);
}

const char * type_name_get(Type *t)
{
	return t->name;
}

void type_instance_property_pointers_get(Type *t, Property *prop, void *instance,
		void **curr, void **prev, char **changed)
{
	Type *pt = property_type_get(prop);
	ssize_t coffset = property_curr_offset_get(prop);
	ssize_t poffset = property_prev_offset_get(prop);
	ssize_t choffset = property_changed_offset_get(prop);

	int offset = type_public_size_get(t) + type_private_size_get(pt->parent);
	*curr = (char *)instance + offset + coffset;
	*prev = (char *)instance + offset + poffset;
	*changed = (char *)instance + offset + choffset;
}

struct _Property_Iterator
{
	Eina_Iterator *it;
	Type *t;
};

Property_Iterator * type_property_iterator_new(Type *t)
{
	Property_Iterator *pit = malloc(sizeof(Property_Iterator));
	pit->t = t;
	pit->it = eina_hash_iterator_data_new(t->properties);

	return pit;
}

Eina_Bool type_property_iterator_next(Property_Iterator *pit, Property **prop)
{
	/* return false on no more properties */
	if (!eina_iterator_next(pit->it, (void **)prop))
	{
		do
		{
			pit->t = pit->t->parent;
			eina_iterator_free(pit->it);
			if (!pit->t)
				return EINA_FALSE;
			pit->it = eina_hash_iterator_data_new(pit->t->properties);
		} while (!eina_iterator_next(pit->it, (void **)prop));

		return EINA_TRUE;
	}
	return EINA_TRUE;
}

void type_property_iterator_free(Property_Iterator *pit)
{
	eina_iterator_free(pit->it);
	free(pit);
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new Type.
 *
 * @param name the type's unique name.
 * @param size the type's size.
 * @param parent the type's parent, or <code>null</code> if it doesnt have one.
 * @param ctor the type's constuctor.
 * @param dtor the type's destructor.
 * @return the newly created type.
 */
Type *type_new(char *name, size_t size, size_t priv_size, Type *parent,
		Type_Constructor ctor, Type_Destructor dtor)
{
	Type *type;

	type = malloc(sizeof(Type));
	if (!type) return NULL;

	/* initialize the type */
	type->name = strdup(name);
	type->size = size;
	type->priv_size = priv_size;
	type->parent = parent;
	type->ctor = ctor;
	type->dtor = dtor;
	type->properties = eina_hash_string_superfast_new(NULL);
	/* add the type */
	if (!_types) _types = eina_hash_string_superfast_new(NULL);
	eina_hash_add(_types, name, type);

	return type;
}

/**
 * Creates a new instance of the given type.
 *
 * @param type the Type to instantiate.
 * @return the instantiated type.
 */
void *type_instance_new(Type *type)
{
	void *instance;

	if (!type)
		return NULL;
	instance = malloc(type_size_get(type));
	if (!instance)
		return NULL;
	object_construct(type, instance);
	return instance;
}

void * type_instance_new_name_from(const char *name)
{
	Type *t;

	t = eina_hash_find(_types, name);
	return type_instance_new(t);
}
/**
 *
 * @param instance
 */
void type_instance_delete(void *instance)
{
	Type *type;
	if (!instance)
		return;
	type = object_private_type_get(instance);
	if (!type)
		return;
	type_destruct_internal(type, instance);
}

/**
 *
 * @param type
 * @param prop_name
 * @param prop_type
 * @param value_type
 * @param process_cb
 */
Property_Id type_property_new(Type *type, char *prop_name,
		Type_Property_Type prop_type, Value_Type value_type,
		ssize_t curr_offset, ssize_t prev_offset, ssize_t changed_offset)
{
	Property *property;

	/* How to handle the changed thing?
	 * usually you do foo_changed:1
	 * but is impossible to get offset of a bit field (man offsetof)
	 * why dont use an uint32/64_t as a bit mask?
	 *
	 * We dont need the process function, just a generic callback handler
	 * for a change after the curr == prev comparision in case it has changed
	 *
	 */
	if (!type || !prop_name)
		return 0;

	property = property_new(type, prop_name, prop_type, value_type,
			curr_offset, prev_offset, changed_offset);
	eina_hash_add(type->properties, prop_name, property);

	return property_id_get(property);
}

EAPI void * type_instance_private_get(Type *t, void *instance)
{
	Type *final;

	final = object_private_type_get(instance);
	return type_instance_private_get_internal(final, t, instance);
}

/**
 * TODO make a function that instead of receiving the name
 * should also recieve the type itself to avoid the strcmp()
 */
EAPI Eina_Bool type_instance_is_of(void *instance, const char *type)
{
	Type *t;

	t = object_private_type_get(instance);
	do
	{
		if (!strcmp(t->name, type))
			return EINA_TRUE;
		t = t->parent;
	}
	while (t);

	return EINA_FALSE;
}
