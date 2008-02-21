#ifndef _CODELANG_CLASS_H
#define _CODELANG_CLASS_H

#include "Codelang.h"

typedef struct class_s class_t;

struct class_s
{
	char *name;
	access_t access;
	list_t *methods;
	list_t *members;
};

typedef enum 
{
	CLASS_ALREADY_DEFINED,
	CLASS_INVALID_NAME,
	CLASS_NOERROR
} class_error_t;

int class_parse(char *name);
void class_access_set(class_t class, access_t access);

#endif
