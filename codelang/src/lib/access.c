#include <string.h>

#include "Codelang.h"

extern class_t last_class;
extern member_t last_member;

int access_parse(char *name, in_t section)
{
	access_t access;

	if (!name) return ACCESS_INVALID_NAME;

	if (!strcmp(name, "public")) access = ACCESS_PUBLIC;
	else if (!strcmp(name, "protected")) access = ACCESS_PROTECTED;
	else if (!strcmp(name, "private")) access = ACCESS_PRIVATE;

	printf("parse_access: %s\n", name);

	switch(section)
	{
		case IN_CLASS:
			class_access_set(last_class, access); 
			break;
		case IN_MEMBER:
			member_access_set(last_member, access);
			break;
		default:
			return ACCESS_INVALID_NAME;
			break;
	}
	return ACCESS_NOERROR;
}

