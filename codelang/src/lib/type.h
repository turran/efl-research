#ifndef _CODELANG_TYPE_H
#define _CODELANG_TYPE_H

typedef enum
{
	TYPE_INT,
	TYPE_CHAR
} type_t;

int type_parse(char *name);

#endif
