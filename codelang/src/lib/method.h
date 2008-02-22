#ifndef _CODELANG_METHOD_H
#define _CODELANG_METHOD_H

#include "Codelang.h"

typedef struct method_s method_t;

struct method_s
{
	char *name;
	type_t ret_type;
	access_t access;
	hash_t *params;
	hash_t *vars;
	list_t *modifiers;
	code_t code;
};

typedef enum 
{
	METHOD_ALREADY_DEFINED,
	METHOD_INVALID_NAME,
	METHOD_NOERROR
} method_error_t;

int method_parse(char *name);
int method_type_set(method_t method, type_t type);

#endif
