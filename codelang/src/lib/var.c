#include "Codelang.h"

extern method_t last_method;

int var_parse(char *name, type_t type)
{
	var_t *var;

	if (last_method.vars)
	{
		if (hash_get(last_method.vars, name))
			return VAR_ALREADY_DEFINED;
	}
	
	var = var_new();
	var->name = strdup(name);
	var->type = type;

	last_method.vars = hash_set(last_method.vars, name, var);

	return VAR_NOERROR;
}

void var_type_set(var_t var, char *name)
{
	type_t type;

	type = type_from_string(name);

	/* type might not be defined yet */
	if (type == TYPE_UNKNOWN)
	{
		/* add to a list and resolve later */
	}
	else
	{
		var.type = type;
	}
}
