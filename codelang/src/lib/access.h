#ifndef _CODELANG_ACCESS_H
#define _CODELANG_ACCESS_H

#include "Codelang.h"

typedef enum 
{
	ACCESS_PUBLIC,
	ACCESS_PROTECTED,
	ACCESS_PRIVATE
} access_t;

typedef enum
{
	ACCESS_INVALID_NAME,
	ACCESS_NOERROR
} access_error_t;

int access_parse(char *name, in_t section);

#endif
