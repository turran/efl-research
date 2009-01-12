#ifndef _OBJECT_H
#define _OBJECT_H

#define OBJECT_ID_CHANGED "idChanged"
extern Property_Id OBJECT_ID_ID;

typedef struct _Object_Private Object_Private;

struct _Object
{
	Eina_Bool (*appendable)(const char *name);
	Object_Private *private;
};

Object *object_new(void);

void object_id_set(Object *object, const char *name);
const char *object_id_get(Object *object);

Type *object_type_get(void);

void object_property_value_set(Object *object, char *prop_name, Value *value);
void object_property_value_get(Object *object, char *prop_name, Value *value);
EAPI void object_child_append(Object *p, Object *o);
EAPI void object_child_remove(Object *p, Object *o);
EAPI void object_event_dispatch(const Object *object, Event *evt);
EAPI const char * object_type_name_get(const Object *obj);

#endif
