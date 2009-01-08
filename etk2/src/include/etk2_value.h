/*
 * etk_value.h
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */

#ifndef ETK_VALUE_H_
#define ETK_VALUE_H_

/**
 * @brief Possible types of a property.
 */
typedef enum
{
	PROPERTY_INT,            /**< The value of the property is an integer */          //!< PROPERTY_INT
	PROPERTY_BOOL,           /**< The value of the property is a boolean (Bool) */    //!< PROPERTY_BOOL
	PROPERTY_CHAR,           /**< The value of the property is a char */              //!< PROPERTY_CHAR
	PROPERTY_FLOAT,          /**< The value of the property is a float */             //!< PROPERTY_FLOAT
	PROPERTY_DOUBLE,         /**< The value of the property is a double */            //!< PROPERTY_DOUBLE
	PROPERTY_SHORT,          /**< The value of the property is a short */             //!< PROPERTY_SHORT
	PROPERTY_LONG,           /**< The value of the property is a long */              //!< PROPERTY_LONG
	PROPERTY_POINTER,        /**< The value of the property is a pointer (void *) */  //!< PROPERTY_POINTER
	PROPERTY_STRING                                                                   //!< PROPERTY_STRING
} Type_Property_Value_Type;

/**
 * @brief
 */
typedef struct _Type_Property_Value
{
	union
	{
		int int_value;
		int bool_value:1;
		char char_value;
		float float_value;
		double double_value;
		short short_value;
		long long_value;
		void *pointer_value;
		char *string_value;
	} value;
} Type_Property_Value;;

static inline void value_int_from(Type_Property_Value *v, int i)
{
	v->value.int_value = i;
}

static inline void value_str_from(Type_Property_Value *v, char *str)
{
	v->value.string_value = str;
}

#endif /* ETK_VALUE_H_ */