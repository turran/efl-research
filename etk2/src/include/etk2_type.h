#ifndef _TYPE_H
#define _TYPE_H

#include "etk2_types.h"

/**
 * @brief
 */
struct _Type_Property_Value
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
};

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
 * @brief A property can either have a single or dual state.
 */
typedef enum
{
	PROPERTY_VALUE_SINGLE_STATE,/**< Single state property */ //!< PROPERTY_VALUE_SINGLE_STATE
	PROPERTY_VALUE_DUAL_STATE   /**< Dual state property */   //!< PROPERTY_VALUE_DUAL_STATE
} Type_Property_Type;

Type *type_new(char *name, size_t size, size_t priv_size, Type *parent, Type_Constructor ctor, Type_Destructor dtor, Object_Property_Value_Set prop_value_set, Object_Property_Value_Get prop_value_get);
void * type_instance_new_name_from(const char *name);
void *type_instance_new(Type *type);
void type_instance_delete(void *instance);
void type_property_new(Type *type, char *prop_name, Type_Property_Type prop_type, Type_Property_Value_Type value_type, size_t field_offset, Type_Property_Process *process_cb);
Type_Property *type_property_get(Type *type, char *prop_name);
void type_instance_property_value_set(Type *type, void *instance, char *prop_name, Type_Property_Value *value);
void * type_instance_private_get(Type *type, void *instance);

#endif
