/*
 * etk2_property.c
 *
 *  Created on: 09-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/**
 * @brief
 */
struct _Property
{
	char *name;
	Type_Property_Type prop_type;
	ssize_t offset;
	Value_Type value_type;
	Type_Property_Process *process;
	Type *type;
	Property_Id id;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Property * property_new(Type *type, char *prop_name, Type_Property_Type prop_type,
		Value_Type value_type, size_t field_offset,
		Type_Property_Process *process_cb)
{
	Property *property;
	static Property_Id id = 0;

	printf("[type] new %s property at offset %d with id %d\n", prop_name, field_offset, id + 1);
	property = malloc(sizeof(Property));
	property->name = strdup(prop_name);
	property->prop_type = prop_type;
	property->value_type = value_type;
	property->process = process_cb;
	property->offset = field_offset;
	property->type = type;
	property->id = ++id;

	return property;
}

Type * property_type_get(Property *p)
{
	return p->type;
}
ssize_t property_offset_get(Property *p)
{
	return p->offset;
}
Value_Type property_value_type_get(Property *p)
{
	return p->value_type;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Property_Id property_id_get(const Property *p)
{
	return p->id;
}

EAPI const char * property_name_get(const Property *p)
{
	return p->name;
}
