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
		Type_Constructor ctor, Type_Destructor dtor, Type_Appendable append);
void * type_instance_new_name_from(const char *name);
void *type_instance_new(Type *type);
void type_instance_delete(void *instance);
Property_Id type_property_new(Type *type, char *prop_name,
		Type_Property_Type prop_type, Value_Type value_type,
		ssize_t curr_offset, ssize_t prev_offset, ssize_t changed_offset);
void * type_instance_private_get(Type *type, void *instance);
EAPI Eina_Bool type_instance_is_of(void *instance, const char *type);

/* TODO add some macros to make easier the addition of single and double properties */
#define TYPE_PROP_SINGLE_ADD(t, name, vt, fo)  \
	type_property_new(t, name, PROPERTY_VALUE_SINGLE_STATE, vt, fo, 0, 0)
#define TYPE_PROP_DOUBLE_ADD(t, name, vt, co, po, cho) \
	type_property_new(t, name, PROPERTY_VALUE_DUAL_STATE, vt, co, po, cho)
#endif
