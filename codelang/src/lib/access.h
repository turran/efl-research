#ifndef _CODELANG_ACCESS_H
#define _CODELANG_ACCESS_H

#include "Codelang.h"

typedef enum 
{
	ACCESS_PUBLIC,
	ACCESS_PROTECTED,
	ACCESS_PRIVATE,
	ACCESS_INVALID_NAME,
	ACCESS_NOERROR
} access_t;

access_t access_parse(char *name);

#endif
