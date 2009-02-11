#ifndef EKEKO_OBJECT_H
#define EKEKO_OBJECT_H

#define OBJECT_ID_CHANGED "idChanged"
extern Property_Id EKEKO_OBJECT_PROPERTY_ID;

typedef struct _Ekeko_Object_Private Ekeko_Object_Private;

struct _Ekeko_Object
{
	Ekeko_Object_Private *private;
};

Ekeko_Object *ekeko_object_new(void);

void ekeko_object_id_set(Ekeko_Object *object, const char *name);
const char *ekeko_object_id_get(Ekeko_Object *object);

Ekeko_Type *ekeko_object_type_get(void);

void ekeko_object_property_value_set(Ekeko_Object *object, char *prop_name, Ekeko_Value *value);
void ekeko_object_property_value_get(Ekeko_Object *object, char *prop_name, Ekeko_Value *value);
EAPI void ekeko_object_child_append(Ekeko_Object *p, Ekeko_Object *o);
EAPI void ekeko_object_child_remove(Ekeko_Object *p, Ekeko_Object *o);
EAPI void ekeko_object_event_dispatch(const Ekeko_Object *object, Event *evt);
EAPI const char * ekeko_object_type_name_get(const Ekeko_Object *obj);
EAPI Ekeko_Object * ekeko_object_parent_get(Ekeko_Object *o);
EAPI Property * ekeko_object_property_get(Ekeko_Object *o, const char *prop_name);
EAPI void ekeko_object_process(Ekeko_Object *o);

#endif
