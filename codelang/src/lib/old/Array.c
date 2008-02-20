#include "objects_list.h"
#include "Array.h"

static void array_append(Array *self, Object *object);
static void array_ctor(Array *self);
static void array_delete(Array **self);
static void array_dtor(Array *self);
static Object *array_get(Array *self, int place);
static int array_length(Array *self);
static Array *array_new();
static int array_put(Array *self, int place, Object *object);

Array_T Array_Class = {
	array_append,
	array_ctor,
	array_delete,
	array_dtor,
	array_get,
	array_length,
	array_new,
	array_put
};

static Array *array_new()
{
	Array *array;

	array = mem_alloc(1, sizeof(Array));
	objects_list_append(OBJECT(array));
	Array_Class.ctor(array);

	return array;
}

static void array_append(Array *self, Object *object)
{
	if (!self)
		return;

	self->elements = clist_append(self->elements, object);
}

static void array_ctor(Array *self)
{
	Object *object;

	object = OBJECT(self);
	Object_Class.ctor(object);

	self->class = &Array_Class;
}

static void array_delete(Array **self)
{
	if (!self || !*self)
		return;

	mem_free(*self);
	*self = NULL;
}

static void array_dtor(Array *self)
{
	Object *object;

	if (!self)
		return;

	if (self->elements)
	{
		CList *l;

		for (l = self->elements; l; l = l->next)
		{
			object = l->data;
			if (!object) 
			{
				self->elements = clist_delete_list(self->elements, l);
				continue;
			}
			OBJECT_REF_DEC(object);
			self->elements = clist_delete(self->elements, object);
		}
	}

	object = OBJECT(self);
	Object_Class.dtor(object);
}

static Object *array_get(Array *self, int place)
{
	if (!self || !self->elements || clist_count(self->elements) < place + 1)
		return NULL;

	return clist_get_at(self->elements, place);
}

static int array_length(Array *self)
{
	if (!self || !self->elements)
		return 0;

	return clist_count(self->elements);
}

static int array_put(Array *self, int place, Object *object)
{
	if (!self)
		return 0;

	if (!self->elements && place > 0)
		return 0;

	/* trying to append after array end */
	if (self->elements && clist_count(self->elements) < place + 1)
		return 0;

	/* list has same or more elements than place */
	if (clist_count(self->elements) <= place)
	{
		Object *old_value = NULL;
		
		if (object)
			OBJECT_REF_INC(object);

		 old_value = clist_replace_at(self->elements, place, object);	

		if (old_value)
			OBJECT_REF_DEC(old_value);

		return 1;
	}

	return 0;
}
