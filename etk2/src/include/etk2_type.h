#ifndef _TYPE_H
#define _TYPE_H

/**
 * @brief A property can either have a single or dual state.
 */
typedef enum
{
	PROPERTY_VALUE_SINGLE_STATE,/**< Single state property */ //!< PROPERTY_VALUE_SINGLE_STATE
	PROPERTY_VALUE_DUAL_STATE   /**< Dual state property */   //!< PROPERTY_VALUE_DUAL_STATE
} Type_Property_Type;

Type *type_new(char *name, size_t size, size_t priv_size, Type *parent,
		Type_Constructor ctor, Type_Destructor dtor);
void * type_instance_new_name_from(const char *name);
void *type_instance_new(Type *type);
void type_instance_delete(void *instance);
Property_Id type_property_new(Type *type, char *prop_name, Type_Property_Type prop_type, Value_Type value_type, size_t field_offset, Type_Property_Process *process_cb);
void * type_instance_private_get(Type *type, void *instance);
EAPI Eina_Bool type_instance_is_of(void *instance, const char *type);

#endif
