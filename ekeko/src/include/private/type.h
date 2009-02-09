/*
 * type.h
 *
 *  Created on: 30-ene-2009
 *      Author: jl
 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

#ifndef TYPE_H_
#define TYPE_H_

Eina_Bool type_appendable(Ekeko_Type *t, void *instance, void *child);
void type_construct(Ekeko_Type *t, void *instance);
void * type_instance_private_get_internal(Ekeko_Type *final, Ekeko_Type *t, void *instance);
Eina_Bool type_instance_property_value_set(Ekeko_Type *type, void *instance, char *prop_name, Value *value, Value *old);
void type_instance_property_value_get(Ekeko_Type *type, void *instance, char *prop_name, Value *value);
Property * type_property_get(Ekeko_Type *t, const char *name);
const char * type_name_get(Ekeko_Type *t);
void type_instance_property_pointers_get(Ekeko_Type *t, Property *prop, void *instance,
		void **curr, void **prev, char **changed);

typedef struct _Property_Iterator Property_Iterator;
Property_Iterator * type_property_iterator_new(Ekeko_Type *t);
Eina_Bool type_property_iterator_next(Property_Iterator *pit, Property **prop);
void type_property_iterator_free(Property_Iterator *pit);

#endif /* TYPE_H_ */
