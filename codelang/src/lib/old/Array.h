#ifndef _CODE_ARRAY_H
#define _CODE_ARRAY_H

#include "clist.h"
#include "Object.h"

#define ARRAY(ptr) ((Array*)(ptr));
#define ARRAY_CLASS Array_Class

typedef struct _Array Array;
typedef struct _Array_T Array_T;

extern Array_T Array_Class;

struct _Array_T
{
	void (*append)(Array *self, Object *object);
	void (*ctor)(Array *self);
	void (*delete)(Array **self);
	void (*dtor)(Array *self);
	Object *(*get)(Array *self, int place);
	int (*length)(Array *self);
	Array *(*new)(void);
	int (*put)(Array *self, int place, Object *object);
};

struct _Array
{
	Object *parent;
	Array_T *class;

	CList *elements;
};

#endif
