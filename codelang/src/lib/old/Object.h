#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "memory_allocator.h"

#define OBJECT(ptr) ((Object*)(ptr))
#define OBJECT_CLASS Object_Class
#define OBJECT_REF_INC(ptr) (OBJECT(ptr))->ref_count++;
#define OBJECT_REF_DEC(ptr) (OBJECT(ptr))->ref_count--;

typedef struct _Object Object;
typedef struct _Object_T Object_T;

extern Object_T Object_Class;

struct _Object_T
{
	void (*ctor)(Object *self);
	void (*delete)(Object **self);
	void (*dtor)(Object *self);
	void (*null)(Object **self);
};

struct _Object
{
	Object_T *class;
	int ref_count;
};

Object *object_new(void);

#endif
