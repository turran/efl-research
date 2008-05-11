/** @file eobj_object.h */
#ifndef EOBJ_OBJECT_H_
#define EOBJ_OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* TODO/FIXME list:
 * - eobj_object_type_get() and eina_object_object_type_get() are confusing: maybe we should rename them
 * - instead of having one list for all the signal-callbacks, we could maybe use one list per type of signal. It
 * would make things more optimized
 * - Dont use hash for the notification-callbacks, a hash is probably to big just for that
 */

/**
 * @defgroup Eobj_Object Eina_Object
 * @brief The Eobj_Object class is the base class for all the objects and widgets of Eina
 * @{
 */

#ifndef EOBJ_DISABLE_CAST_CHECKS
#define EOBJ_OBJECT_CAST(obj, eina_type, c_type) \
      ((c_type *)eobj_object_check_cast((Eobj_Object *)(obj), (eina_type)))
#else
#define EOBJ_OBJECT_CAST(obj, eina_type, c_type)     ((c_type *)(obj))
#endif

#define EOBJ_OBJECT_CHECK_TYPE(obj, eina_type) \
   (eina_type_inherits_from(((Eobj_Object *)(obj))->type, (eina_type)))

/** Gets the type of an object */
#define EOBJ_OBJECT_TYPE       (eobj_object_type_get())
/** Casts the object to an Eobj_Object */
#define EOBJ_OBJECT(obj)       (EINA_OBJECT_CAST((obj), EINA_OBJECT_TYPE, Eobj_Object))
/** Checks if the object is an Eobj_Object */
#define EOBJ_IS_OBJECT(obj)    (EINA_OBJECT_CHECK_TYPE((obj), EINA_OBJECT_TYPE))

/**
 * @brief A callback called each time the value of the associated property is modified
 * @structinfo
 */
struct _Eobj_Notification_Callback {
	/* private: */
	void (*callback)(Eobj_Object *object, const char *property_name,
			void *data);
	void *data;
	Eina_Bool delete_me :1;
};

/**
 * @brief Symbols for signals
 */

extern int EOBJ_OBJECT_DESTROYED_SIGNAL;

/**
 * @brief @object The base class for all the objects and widgets of Eina
 * @structinfo
 */
struct _Eobj_Object {
	/* private: */
	Eobj_Class *type;
	char *name;

	Eobj_Object *prev;
	Eobj_Object *next;

	Eina_Hash *data_hash;
	Eina_List **signal_callbacks;
	Eina_List *weak_pointers;
	Eina_Hash *notification_callbacks;
	int notifying;
	Eina_Bool should_delete_cbs :1;
	Eina_Bool destroy_me :1;
};

void eobj_object_shutdown(void);
void eobj_object_purge(void);

Eobj_Class *eobj_object_type_get(void);
Eobj_Object *eobj_object_new(Eobj_Class *object_type,
		const char *first_property, ...);
Eobj_Object *eobj_object_new_valist(Eobj_Class *object_type,
		const char *first_property, va_list args);
void eobj_object_destroy(Eobj_Object *object);

void eobj_object_name_set(Eobj_Object *object, const char *name);
const char *eobj_object_name_get(Eobj_Object *object);
Eobj_Object *eobj_object_name_find(const char *name);

Eobj_Object *eobj_object_check_cast(Eobj_Object *object, Eobj_Class *type);
Eobj_Class *eobj_object_object_type_get(Eobj_Object *object);

void eobj_object_signal_callback_add(Eobj_Object *object, int signal_code,
		Eobj_Signal_Callback *signal_callback, Eina_Bool after);
void eobj_object_signal_callback_remove(Eobj_Object *object, int signal_code,
		Eobj_Signal_Callback *signal_callback);
void eobj_object_signal_callbacks_get(Eobj_Object *object, int signal_code,
		Eina_List **callbacks);

void eobj_object_weak_pointer_add(Eobj_Object *object, void **pointer_location);
void eobj_object_weak_pointer_remove(Eobj_Object *object,
		void **pointer_location);

void eobj_object_data_set(Eobj_Object *object, const char *key, void *value);
void eobj_object_data_set_full(Eobj_Object *object, const char *key,
		void *value, void (*free_cb)(void *data));
void *eobj_object_data_get(Eobj_Object *object, const char *key);

void eobj_object_property_reset(Eobj_Object *object, const char *property_name);
void eobj_object_properties_set(Eobj_Object *object,
		const char *first_property, ...);
void eobj_object_properties_set_valist(Eobj_Object *object,
		const char *first_property, va_list args);
void eobj_object_properties_get(Eobj_Object *object,
		const char *first_property, ...);
void eobj_object_properties_get_valist(Eobj_Object *object,
		const char *first_property, va_list args);

void eobj_object_notify(Eobj_Object *object, const char *property_name);
void eobj_object_notification_callback_add(Eobj_Object *object, const char *property_name,
				void (*callback)(Eobj_Object *object, const char *property_name, void *data),
				void *data);
void eobj_object_notification_callback_remove(Eobj_Object *object, const char *property_name,
				void (*callback)(Eobj_Object *object, const char *property_name, void *data));

/** @} */

#ifdef __cplusplus
}
#endif

#endif
