#include <stdlib.h>

#include "Object.h"
#include "objects_list.h"

static void object_ctor(Object *self);
static void object_delete(Object **self);
static void object_dtor(Object *self);
static void object_null(Object **self);

Object_T Object_Class = {
	object_ctor,
	object_delete,
	object_dtor,
	object_null
};

Object *object_new(void)
{
	Object *object;

	object = mem_alloc(1, sizeof(Object));
	objects_list_append(object);
	Object_Class.ctor(object);

	return object;
}

static void object_ctor(Object *self)
{	
	self->ref_count = 1;
	self->class = &Object_Class;
}

static void object_dtor(Object *self)
{
	if (!self)
		return;

	self->ref_count = 0;
}

static void object_delete(Object **self)
{
	if (!self || !*self)
		return;

	mem_free(*self);
	*self = NULL;
}

static void object_null(Object **self)
{
	if (!self || !*self)
		return;

	OBJECT_REF_DEC(*self);
	*self = NULL;
}
