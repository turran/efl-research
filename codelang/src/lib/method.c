#include <string.h>

#include "Codelang.h"

method_t last_method;

int method_parse(char *name)
{
	printf("method_parse: '%s'\n", name);
	last_method.name = strdup(name);
	return METHOD_NOERROR;
}

int method_type_set(method_t method, type_t type)
{
	method.ret_type = type;
	return 1;
}
