#ifndef _CODE_OBJLIST_H
#define _CODE_OBJLIST_H

#include "clist.h"
#include "Object.h"

CList *objects_list_get(void);
void objects_list_append(Object *object);
void objects_list_delete(Object *object);

#endif
