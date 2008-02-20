#ifndef _CODE_CCLASS_H
#define _CODE_CCLASS_H

#define CCLASS(ptr) ((CClass)(ptr))

typedef struct _CClass CClass;

struct _CClass
{
	char *string_name;
	int id;
};

CClass *cl_class_lookup(char *name);

#endif
