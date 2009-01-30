/*
 * etk2_property.h
 *
 *  Created on: 09-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_PROPERTY_H_
#define EKEKO_PROPERTY_H_

typedef struct _Property Property;
typedef uint32_t Property_Id;

EAPI Property_Id ekeko_property_id_get(const Property *p);
EAPI const char * ekeko_property_name_get(const Property *p);
Value_Type ekeko_property_value_type_get(Property *p);

#endif /* EKEKO_PROPERTY_H_ */
