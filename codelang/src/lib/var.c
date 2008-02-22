#include <string.h>

#include "Codelang.h"

extern method_t last_method;
var_t last_var;

var_t *var_new()
{
	var_t *var;
	var = calloc(1, sizeof(var_t));
	return var;
}
int var_parse(char *name)
{
	var_t *var;

	if (last_method.vars)
	{
		if (hash_find(last_method.vars, name))
			return VAR_ALREADY_DEFINED;
	}
	
	var = var_new();
	var->name = strdup(name);

	last_method.vars = hash_add(last_method.vars, name, var);

	return VAR_NOERROR;
}

int var_type_set(var_t var, type_t type)
{
	var.type = type;
	return 1;
}
