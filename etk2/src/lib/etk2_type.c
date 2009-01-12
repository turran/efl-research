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

static void * _instance_property_offset_get(Type *type, Property *prop, void *instance)
{
	Type *pt = property_type_get(prop);
	ssize_t poffset = property_offset_get(prop);

	int offset = type_public_size_get(type) + type_private_size_get(pt->parent);
	return (char *)instance + offset + poffset;
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
/**
 *
 * @param type
 * @param instance
 * @param prop_name
 * @param value
 */
Eina_Bool type_instance_property_value_set(Type *type, void *instance, char *prop_name, Value *value, Value *old)
{
	void *curr;
	Property *property;

	if (type == NULL || instance == NULL || prop_name == NULL || value == NULL)
		return EINA_FALSE;
	property = _property_get(type, prop_name);
	if (!property)
		return EINA_FALSE;
	curr = _instance_property_offset_get(type, property, instance);

	if (property_value_type_get(property) == PROPERTY_STRING)
	{
		//printf("[type] property->type->name = %s with size = %d\n", property->type->name, property->type->size);
		char **str = (char**)curr;
		if (old) old->value.string_value = *str;
		*str = strdup(value->value.string_value);
	}
	return EINA_TRUE;
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
	curr = _instance_property_offset_get(type, property, instance);
	value_set(v, property_value_type_get(property), curr);
}

const char * type_name_get(Type *t)
{
	return t->name;
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
		size_t field_offset, Type_Property_Process *process_cb)
{
	Property *property;

	if (!type || !prop_name)
		return 0;

	property = property_new(type, prop_name, prop_type, value_type, field_offset, process_cb);
	eina_hash_add(type->properties, prop_name, property);

	return property_id_get(property);
}

#if 0
void type_property_new(Type *type, char *prop_name,
		Type_Property_Type prop_type, Value_Type value_type,
		ssize_t field_offset, ssize_t prev_offset, ssize_t changed)
{
	/* How to handle the changed thing?
	 * usually you do foo_changed:1
	 * but is impossible to get offset of a bit field (man offsetof)
	 * why dont use an uint32/64_t as a bit mask?
	 *
	 * We dont need the process function, just a generic callback handler
	 * for a change after the curr == prev comparision in case it has changed
	 *
	 */
}
#endif

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
