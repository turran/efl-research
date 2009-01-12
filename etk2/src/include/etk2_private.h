/*
 * etk_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef _ETK2_PRIVATE_H
#define _ETK2_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

/* object global */
Type * object_private_type_get(Object *object);
void object_construct(Type *type, void *instance);
void object_event_listener_add(Object *obj, const char *type, Event_Listener el, Eina_Bool bubble);
void object_event_listener_remove(Object *obj, const char *type, Event_Listener el);
/* type global */
void type_construct(Type *t, void *instance);
void * type_instance_private_get_internal(Type *final, Type *t, void *instance);
Eina_Bool type_instance_property_value_set(Type *type, void *instance, char *prop_name, Value *value, Value *old);
void type_instance_property_value_get(Type *type, void *instance, char *prop_name, Value *value);
Property * type_property_get(Type *t, const char *name);
const char * type_name_get(Type *t);

/* value global */
void value_set(Value *v, Value_Type vtype, void *val);
/* event global */
void event_mutation_init(Event_Mutation *em, const char *type, const Object *o,
		const Object *rel, const Property *prop, Value *prev, Value *curr);
/* property global */
Property * property_new(Type *type, char *prop_name, Type_Property_Type prop_type,
		Value_Type value_type, size_t field_offset,
		Type_Property_Process *process_cb);
Type * property_type_get(Property *p);
ssize_t property_offset_get(Property *p);
Value_Type property_value_type_get(Property *p);

#endif
