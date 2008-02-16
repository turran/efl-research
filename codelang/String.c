#include <string.h>
#include <stdlib.h>

#include "objects_list.h"
#include "Object.h"
#include "String.h"

static void string_assign(String *self, String *str);
static void string_ctor(String *self);
static void string_delete(String **self);
static void string_dtor(String *self);
static void string_clone(String *self, String *str);
static int string_length(String *self);
static void string_set(String *self, char *str);
static char *string_to_string(String *self);

String_T String_Class = {
	string_assign,
	string_ctor,
	string_clone,
	string_delete,
	string_dtor,
	string_length,
	string_set,
	string_to_string
};

String *string_new(void)
{
	String *string;
	
	string = mem_alloc(1, sizeof(String));
	objects_list_append(OBJECT(string));
	String_Class.ctor(string);

	return string;
}

static void string_assign(String *self, String *str)
{
	if (!self)
		return;

	if (self->value)
		free(self->value);

	if (!str)
		self->value = NULL;
	else
	{
		self->value = str->value;
		OBJECT_REF_INC(str);
	}
}

static void string_ctor(String *self)
{
	Object *object;

	object = OBJECT(self);
	Object_Class.ctor(object);	

	self->class = &String_Class;
}

static void string_clone(String *self, String *str)
{
	if (!self || !str || !str->value)
		return;

	if (self->value)
		free(self->value);
	
	self->value = strdup(str->value);
}

static void string_delete(String **self)
{
	if (!self || !*self)
		return;
	
	mem_free(*self);
	*self = NULL;
}

static void string_dtor(String *self)
{
	Object *object;

	if (!self)
		return;

	if (self->value)
		free(self->value);
	
	self->value = NULL;

	object = OBJECT(self);
	Object_Class.dtor(object);
}

static int string_length(String *self)
{
	if (!self || !self->value)
		return 0;

	return strlen(self->value);	
}

static void string_set(String *self, char *str)
{
	if (!self)
		return;

	if (self->value)
		free(self->value);

	self->value = str ? strdup(str) : NULL;
}

static char *string_to_string(String *self)
{
	if (!self)
		return NULL;

	return self->value;
}
