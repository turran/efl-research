#ifndef _CODELANG_TYPE_H
#define _CODELANG_TYPE_H

typedef enum
{
	TYPE_UNKOWN,
	TYPE_INT,
	TYPE_CHAR,
	TYPE_UNKNOWN
} type_t;

type_t type_parse(char *name);

#endif
