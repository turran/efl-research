#include <string.h>

#include "Codelang.h"

access_t access_parse(char *name)
{
	access_t access = ACCESS_INVALID_NAME;

	printf("parse_access: %s\n", name);

	if (!name) return access;

	if (!strcmp(name, "public")) access = ACCESS_PUBLIC;
	else if (!strcmp(name, "protected")) access = ACCESS_PROTECTED;
	else if (!strcmp(name, "private")) access = ACCESS_PRIVATE;

	return access;
}

