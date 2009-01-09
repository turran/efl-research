/*
 * etk2_property.h
 *
 *  Created on: 09-ene-2009
 *      Author: jl
 */
#ifndef ETK2_PROPERTY_H_
#define ETK2_PROPERTY_H_

typedef struct _Property Property;
typedef uint32_t Property_Id;

EAPI Property_Id property_id_get(const Property *p);
EAPI const char * property_name_get(const Property *p);

#endif /* ETK2_PROPERTY_H_ */
