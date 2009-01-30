/*
 * property.h
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

#ifndef PROPERTY_H_
#define PROPERTY_H_

Property * property_new(Ekeko_Type *type, char *prop_name, Type_Property_Type prop_type,
		Value_Type value_type, ssize_t curr_offset, ssize_t prev_offset,
		ssize_t changed_offset);
Ekeko_Type * property_type_get(Property *p);
ssize_t property_curr_offset_get(Property *p);
ssize_t property_prev_offset_get(Property *p);
ssize_t property_changed_offset_get(Property *p);
Type_Property_Type property_ptype_get(Property *p);

#endif /* PROPERTY_H_ */
