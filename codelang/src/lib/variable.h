#ifndef _CODE_VARIABLE_H
#define _CODE_VARIABLE_H

typedef enum
{
	VAR_ALREADY_DEFINED,
	VAR_ILLEGAL_NAME,
	VAR_NOERROR
} var_error_t;  

int parse_variable(char *name);

#endif
