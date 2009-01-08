/*
 * etk_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef _ETK2_PRIVATE_H
#define _ETK2_PRIVATE_H

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

/* object global */
Type * object_private_type_get(Object *object);
void object_construct(Type *type, void *instance);
void object_event_dispatch(Object *obj, Event *e);
void object_event_listener_add(Object *obj, const char *type, Event_Listener el);
void object_event_listener_remove(Object *obj, const char *type, Event_Listener el);
/* type global */
void type_construct(Type *t, void *instance);
void * type_instance_private_get_internal(Type *final, Type *t, void *instance);
Eina_Bool type_instance_property_value_set(Type *type, void *instance, char *prop_name, Type_Property_Value *value, Type_Property_Value *old);
void type_instance_property_value_get(Type *type, void *instance, char *prop_name, Type_Property_Value *value);
/* value global */
void value_set(Type_Property_Value *v, Type_Property_Value_Type vtype, void *val);
/* event global */
void event_mutation_init(Event_Mutation *em, Object *o, const char *prop);

#endif
