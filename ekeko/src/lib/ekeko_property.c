/*
 * etk2_property.c
 *
 *  Created on: 09-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
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
	ssize_t curr_offset;
	ssize_t prev_offset;
	ssize_t changed_offset;
	Ekeko_Value_Type value_type;
	Ekeko_Type *type;
	Property_Id id;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Property * property_new(Ekeko_Type *type, char *prop_name, Type_Property_Type prop_type,
		Ekeko_Value_Type value_type, ssize_t curr_offset, ssize_t prev_offset,
		ssize_t changed_offset)
{
	Property *property;
	static Property_Id id = 0;

#ifdef EKEKO_DEBUG
	printf("[Ekeko_Property] new %s property at offset %d with id %d\n", prop_name, curr_offset, id + 1);
#endif
	property = malloc(sizeof(Property));
	property->name = strdup(prop_name);
	property->prop_type = prop_type;
	property->value_type = value_type;
	property->curr_offset = curr_offset;
	property->prev_offset = prev_offset;
	property->changed_offset = changed_offset;
	property->type = type;
	property->id = ++id;

	return property;
}

/* TODO make this getters inline on private.h */
Ekeko_Type * property_type_get(Property *p)
{
	return p->type;
}
ssize_t property_curr_offset_get(Property *p)
{
	return p->curr_offset;
}

ssize_t property_prev_offset_get(Property *p)
{
	return p->prev_offset;
}

ssize_t property_changed_offset_get(Property *p)
{
	return p->changed_offset;
}


Type_Property_Type property_ptype_get(Property *p)
{
	return p->prop_type;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Property_Id ekeko_property_id_get(const Property *p)
{
	return p->id;
}

EAPI const char * ekeko_property_name_get(const Property *p)
{
	return p->name;
}

EAPI Ekeko_Value_Type ekeko_property_value_type_get(Property *p)
{
	return p->value_type;
}
