#ifndef _CODE_VAR_H
#define _CODE_VAR_H

typedef struct var_s var_t; 

typedef enum
{
	VAR_ALREADY_DEFINED,
	VAR_ILLEGAL_NAME,
	VAR_NOERROR
} var_error_t;  

struct var_s
{
	char *name;
	type_t type;
};

var_t *var_new();
int var_parse(char *name);
int var_type_set(var_t var, type_t type);

#endif
