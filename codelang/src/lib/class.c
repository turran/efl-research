#include "Codelang.h"

class_t last_class;

int class_parse(char *name)
{
	printf("parse_class: %s\n", name);
	return 1;
}

void class_access_set(class_t class, access_t access)
{
	class.access = access;
}

