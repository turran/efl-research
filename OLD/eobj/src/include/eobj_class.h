#ifndef EOBJ_CLASS_H_
#define EOBJ_CLASS_H_

/**
 * @defgroup Eobj_Class Eobj_Class
 * @{
 */

/** Casts the function pointer to an Eobj_Constructor */
#define EOBJ_CONSTRUCTOR(constructor)                ((Eobj_Constructor)(constructor))
/** Casts the function pointer to an Eobj_Constructor */
#define EOBJ_DESTRUCTOR(destructor)                  ((Eobj_Destructor)(destructor))

typedef void (*Eobj_Constructor)(Eobj_Object *object);
typedef void (*Eobj_Destructor)(Eobj_Object *object);
/**
 * @struct Eobj_Class
 * @brief The type of an Eobj_Object: it can have a constructor and a destructor and can inherit from another Eobj_Class @n
 * It also has a list of properties that will describe the state of the instanciated object
 */
struct _Eobj_Class {
	/* private: */
	char *name;
	int hierarchy_depth;
	Eobj_Class **hierarchy;
	Eobj_Constructor constructor;
	Eobj_Destructor destructor;
	void (*property_set)(Eobj_Object *object, int property_id,
			Eobj_Property_Value *value);
	void (*property_get)(Eobj_Object *object, int property_id,
			Eobj_Property_Value *value);
	int type_size;

	unsigned int signals_count;

	Eobj_Signal **signals;
	Eina_Hash *properties_hash;
};


#ifdef __cplusplus
extern "C" {
#endif

void eobj_class_shutdown(void);

Eobj_Class *eobj_class_new(const char *type_name, Eobj_Class *parent_type,
		int type_size, Eobj_Constructor constructor,
		Eobj_Destructor destructor,
		const Eobj_Signal_Description *signals);
void eobj_class_delete(Eobj_Class *type);

void eobj_class_object_construct(Eobj_Class *type, Eobj_Object *object);
void eobj_class_destructors_call(Eobj_Class *type, Eobj_Object *object);
Eina_Bool eobj_class_inherits_from(Eobj_Class *type, Eobj_Class *parent);
Eobj_Class *eobj_class_parent_type_get(Eobj_Class *type);
const char *eobj_class_name_get(Eobj_Class *type);
Eobj_Class *eobj_class_get_from_name(const char *name);

Eobj_Signal *eobj_class_signal_get(Eobj_Class *type, int signal_code);
Eobj_Signal *eobj_class_signal_get_by_name(Eobj_Class *type,
		const char *signal_name);

Eobj_Property *eobj_class_property_add(Eobj_Class *type, const char *name,
		int property_id, Eobj_Property_Type property_type,
		Eobj_Property_Flags flags, Eobj_Property_Value *default_value);
Eina_Bool eobj_class_property_find(Eobj_Class *type, const char *name,
		Eobj_Class **property_owner, Eobj_Property **property);
void eobj_class_property_list(Eobj_Class *type, Eina_List **properties);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
