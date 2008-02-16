#include <unistd.h>

#include "objects_list.h"

static CList *list = NULL;

CList *objects_list_get(void)
{
	return list;
}

void objects_list_append(Object *object)
{
	if (!object)
		return;

	list = clist_append(list, object);
}

void objects_list_delete(Object *object)
{
	list = clist_delete(list, object);
}

