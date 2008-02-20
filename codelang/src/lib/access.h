#ifndef _CODELANG_ACCESS_H
#define _CODELANG_ACCESS_H

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

int parse_access(char *name);

#endif
