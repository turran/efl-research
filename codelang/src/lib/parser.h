#ifndef _CODELANG_PARSER_H
#define _CODELANG_PARSER_H

#include "Codelang.h"

typedef enum
{
	IN_CLASS,
	IN_MEMBER,
	IN_VAR
} in_t;

void file_load(char *file);

#endif
