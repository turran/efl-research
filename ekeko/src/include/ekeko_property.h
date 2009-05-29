/*
 * etk2_property.h
 *
 *  Created on: 09-ene-2009
 *      Author: jl
 */
#ifndef EKEKO_PROPERTY_H_
#define EKEKO_PROPERTY_H_

typedef struct _Ekeko_Property Ekeko_Property;
typedef uint32_t Ekeko_Property_Id;

EAPI Ekeko_Property_Id ekeko_property_id_get(const Ekeko_Property *p);
EAPI const char * ekeko_property_name_get(const Ekeko_Property *p);
Ekeko_Value_Type ekeko_property_value_type_get(Ekeko_Property *p);

#endif /* EKEKO_PROPERTY_H_ */
