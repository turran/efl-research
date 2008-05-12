#include "Eobj.h"
#include "eobj_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eobj_Object *_eobj_object_objects = NULL;
static Eobj_Object *_eobj_object_last_object = NULL;
static Eina_Hash *_eobj_object_name_hash = NULL;

int EOBJ_OBJECT_DESTROYED_SIGNAL;

typedef struct Eobj_Object_Data
{
   void *value;
   void (*free_cb)(void *data);
} Eobj_Object_Data;

enum Eobj_Object_Property_Id
{
   EOBJ_OBJECT_NAME_PROPERTY
};

/* Removes the notification-callbacks marked as "deleted" from the list (called by eobj_object_notify()) */
static Eina_Bool _eobj_object_notification_callbacks_clean_cb(const Eina_Hash *hash, const char *key, void *data, void *fdata)
{
   Eina_List *l, *next;
   Eina_List **callbacks;
   Eobj_Notification_Callback *callback;

   if (!(callbacks = data))
      return 1;

   for (l = *callbacks; l; l = next)
   {
      next = eina_list_next(l);
      callback = eina_list_data(l);

      if (callback->delete_me)
      {
         free(callback);
         *callbacks = eina_list_remove_list(*callbacks, l);
      }
   }

   return 1;
}

/* Frees a list of notification callbacks (called by _eobj_object_destructor()) */
static Eina_Bool _eobj_object_notification_callbacks_free_cb(const Eina_Hash *hash, const char *key, void *data, void *fdata)
{
   Eina_List **list;

   if (!(list = data))
      return 1;

   while (*list)
   {
      free(eina_list_data(*list));
      *list = eina_list_remove_list(*list, *list);
   }
   free(list);

   return 1;
}

/* Frees data from the data hash of the object */
static Eina_Bool _eobj_object_data_free_cb(const Eina_Hash *hash, const char *key, void *data, void *fdata)
{
   Eobj_Object_Data *object_data;

   if (!(object_data = data))
      return 1;

   if (object_data->free_cb)
      object_data->free_cb(object_data->value);
   free(object_data);

   return 1;
}



/* Initializes the object */
static void _eobj_object_constructor(Eobj_Object *object)
{
   if (!object)
      return;

   object->name = NULL;
   object->destroy_me = EINA_FALSE;
   object->data_hash = NULL;
   object->weak_pointers = NULL;
   object->notification_callbacks = NULL;
   object->should_delete_cbs = EINA_FALSE;
   object->notifying = 0;

   /* Append the new object to the list */
   object->prev = _eobj_object_last_object;
   object->next = NULL;
   if (!_eobj_object_objects)
      _eobj_object_objects = object;
   if (_eobj_object_last_object)
      _eobj_object_last_object->next = object;
   _eobj_object_last_object = object;
}

/* Destroys the object */
static void _eobj_object_destructor(Eobj_Object *object)
{
   if (!object)
      return;

   eina_hash_foreach(object->data_hash, _eobj_object_data_free_cb, NULL);
   eina_hash_free(object->data_hash);

   eina_hash_foreach(object->notification_callbacks, _eobj_object_notification_callbacks_free_cb, NULL);
   eina_hash_free(object->notification_callbacks);
}

/* Sets the property whose id is "property_id" to the value "value" */
static void _eobj_object_property_set(Eobj_Object *object, int property_id, Eobj_Property_Value *value)
{
   if (!object || !value)
      return;

   switch (property_id)
   {
      case EOBJ_OBJECT_NAME_PROPERTY:
         eobj_object_name_set(object, eina_property_value_string_get(value));
         break;
      default:
         break;
   }
}

/* Gets the value of the property whose id is "property_id" */
static void _eobj_object_property_get(Eobj_Object *object, int property_id, Eobj_Property_Value *value)
{
   if (!object || !value)
      return;

   switch (property_id)
   {
      case EOBJ_OBJECT_NAME_PROPERTY:
         eina_property_value_string_set(value, object->name);
         break;
      default:
         break;
   }
}


/* Frees the object: it calls the destructors (from the destructor of the more derived class
 * to the destructor of the ultimate base class) and frees the allocated memory */
static void _eobj_object_free(Eobj_Object *object)
{
   if (!object)
      return;

   eobj_object_destroy(object);
   eina_type_destructors_call(object->type, object);

   if (object->prev)
      object->prev->next = object->next;
   if (object->next)
      object->next->prev = object->prev;
   if (object == _eobj_object_objects)
      _eobj_object_objects = object->next;
   if (object == _eobj_object_last_object)
      _eobj_object_last_object = object->prev;

   free(object);
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * @internal
 * @brief Shutdowns the object system: it frees all the created objects
 */
void eobj_object_shutdown(void)
{
   while (_eobj_object_objects)
      _eobj_object_free(_eobj_object_objects);
   eina_hash_free(_eobj_object_name_hash);
   _eobj_object_name_hash = NULL;
}
/**
 * @internal
 * @brief Frees the objects that have been marked as "destroyed".
 * It's called at the start of each iteration of the main loop
 */
void eobj_object_purge(void)
{
   Eobj_Object *object, *next;

   for (object = _eobj_object_objects; object; object = next)
   {
      next = object->next;
      if (object->destroy_me)
         _eobj_object_free(object);
   }
}

/**
 * @internal
 * @brief Gets the type of an Eobj_Object
 * @return Returns the type of an Eobj_Object
 */
Eobj_Class *eobj_object_type_get(void)
{
   static Eobj_Class *object_type = NULL;

   if (!object_type)
   {
      const Eobj_Signal_Description signals[] = {
         EOBJ_SIGNAL_DESC_NO_HANDLER(EOBJ_OBJECT_DESTROYED_SIGNAL,
            "destroyed", eina_marshaller_VOID),
         EOBJ_SIGNAL_DESCRIPTION_SENTINEL
      };

      object_type = eina_type_new("Eobj_Object", NULL,
         sizeof(Eobj_Object), EOBJ_CONSTRUCTOR(_eobj_object_constructor),
         EOBJ_DESTRUCTOR(_eobj_object_destructor), signals);

      eina_type_property_add(object_type, "name", EOBJ_OBJECT_NAME_PROPERTY,
         EOBJ_PROPERTY_STRING, EOBJ_PROPERTY_READABLE_WRITABLE, eina_property_value_string(NULL));

      object_type->property_set = _eobj_object_property_set;
      object_type->property_get = _eobj_object_property_get;
   }

   return object_type;
}

/**
 * @brief Creates a new object: it calls the corresponding constructors (from the constructor of the base class to the
 * constructor of the more derived class) and then sets the values of the given properties
 * @param object_type the type of object to create
 * @param first_property the name of the first property to set
 * @param ... the value of the first property, followed by any number of property-name/property-value pairs,
 * terminated with NULL
 * @return Returns the new Eobj_Object of type @a object_type
 */
Eobj_Object *eobj_object_new(Eobj_Class *object_type, const char *first_property, ...)
{
   Eobj_Object *new_object;
   va_list args;

   if (!object_type)
      return NULL;

   va_start(args, first_property);
   new_object = eobj_object_new_valist(object_type, first_property, args);
   va_end(args);

   return new_object;
}

/**
 * @brief Creates a new object: it calls the corresponding constructors (from the constructor of the base class to the
 * constructor of the more derived class) and then sets the values of the given properties
 * @param object_type the type of object to create
 * @param first_property the name of the first property to set
 * @param args the value of the first property, followed by any number of property-name/property-value pairs,
 * terminated with NULL
 * @return Returns the new Eobj_Object of type @a object_type
 */
Eobj_Object *eobj_object_new_valist(Eobj_Class *object_type, const char *first_property, va_list args)
{
   Eobj_Object *new_object;
   va_list args2;

   if (!object_type)
      return NULL;

   new_object = malloc(object_type->type_size);
   new_object->type = object_type;

   eina_type_object_construct(object_type, new_object);
   va_copy(args2, args);
   eobj_object_properties_set_valist(new_object, first_property, args2);
   va_end(args2);

   return new_object;
}

/**
 * @brief Destroys the object: it first sets the weak-pointers to NULL, emits the "destroyed" signal, and then
 * queues the object in the list of objects to free. Thus, the destructors will only be called at the beginning of the
 * next main loop iteration (from the destructor of the more derived class to the destructor of the ultimate base class).
 * @param object the object to destroy
 * @warning You should not assume that this function will call directly the destructors of the object!
 */
void eobj_object_destroy(Eobj_Object *object)
{
   void **weak_pointer;

   if (!object || object->destroy_me)
      return;

   eobj_object_name_set(object, NULL);

   /* Sets the weak-pointers to NULL */
   while (object->weak_pointers)
   {
      weak_pointer = object->weak_pointers->data;
      *weak_pointer =  NULL;
      object->weak_pointers = eina_list_remove_list(object->weak_pointers, object->weak_pointers);
   }

   object->destroy_me = EINA_TRUE;
   eina_signal_emit(EOBJ_OBJECT_DESTROYED_SIGNAL, object);
}

/**
 * @brief Sets the name of the object. The object can then be retrieved from his name with eobj_object_name_find()
 * @param object an object
 * @param name the name to set
 * @see eobj_object_name_find()
 */
void eobj_object_name_set(Eobj_Object *object, const char *name)
{
   Eobj_Object *object2;

   if (!object || object->destroy_me || object->name == name)
      return;

   if (name)
   {
      if ((object2 = eobj_object_name_find(name)))
      {
         if (object == object2)
            return;
         else
            eobj_object_name_set(object2, NULL);
      }

      free(object->name);
      object->name = strdup(name);
      _eobj_object_name_hash = eina_hash_add(_eina_object_name_hash, object->name, object);
   }
   else
   {
      _eobj_object_name_hash = eina_hash_del(_eina_object_name_hash, object->name, object);
      free(object->name);
      object->name = NULL;
   }

   eobj_object_notify(object, "name");
}

/**
 * @brief Gets the name of the object
 * @param object a object
 * @return Returns the name of the object
 */
const char *eobj_object_name_get(Eobj_Object *object)
{
   if (!object || object->destroy_me)
      return NULL;
   return object->name;
}

/**
 * @brief Finds the object called @a name
 * @param name the name of the object to find
 * @return Returns the object called @a name
 */
Eobj_Object *eobj_object_name_find(const char *name)
{
   if (!name)
      return NULL;
   return EOBJ_OBJECT(eina_hash_find(_eobj_object_name_hash, name));
}

/**
 * @brief Checks if @a object can be cast to @a type.
 * If @a object doesn't inherit from @a type, a warning is displayed in the console but the object is returned anyway.
 * @param object the object to cast
 * @param type the type to which we cast the object
 * @return Returns the object
 * @note You usually do not need to call this function, use specific macros instead (EOBJ_IS_WIDGET() for example)
 */
Eobj_Object *eobj_object_check_cast(Eobj_Object *object, Eobj_Class *type)
{
   if (!object)
      return NULL;

   if (!eina_type_inherits_from(object->type, type))
      EOBJ_WARNING("Invalid cast from \"%s\" to \"%s\"", eina_type_name_get(object->type), eina_type_name_get(type));

   return object;
}

/**
 * @brief Gets the type of the object
 * @param object an object
 * @return Returns the type of @a object (NULL on failure)
 */
Eobj_Class *eobj_object_object_type_get(Eobj_Object *object)
{
   if (!object)
      return NULL;
   return object->type;
}

/**
 * @internal
 * @brief Adds @a signal_callback to the list of the signal-callbacks of the
 *        object.
 *
 * @param object an object
 * @param signal_code the signal identification code
 * @param signal_callback the signal-callback to add
 * @param after if @a after is EINA_TRUE, the callback will be called after all
 *              the other callbacks already connected to this signal, otherwise
 *              it will be called before (default behaviour)
 *
 * @note You do not have to call this function, use eina_signal_connect() instead
 */
void eobj_object_signal_callback_add(Eobj_Object *object, int signal_code,
                                    Eobj_Signal_Callback *signal_callback,
                                    Eina_Bool after)
{
   if (after)
      object->signal_callbacks[signal_code] =
         eina_list_append(object->signal_callbacks[signal_code],
                          signal_callback);
   else
      object->signal_callbacks[signal_code] =
         eina_list_prepend(object->signal_callbacks[signal_code],
                           signal_callback);
}

/**
 * @internal
 * @brief Removes @a signal_callback from the list of the signal-callbacks of
 *        the object.
 *
 * @param object an object
 * @param signal_code the signal identification code
 * @param signal_callback the signal-callback to remove
 *
 * @note You do not have have to call this function, use eina_signal_disconnect()
 *       instead.
 */
void eobj_object_signal_callback_remove(Eobj_Object *object, int signal_code,
                                       Eobj_Signal_Callback *signal_callback)
{
   if (!signal_callback)
      return;

   Eina_List *lst = eina_list_find_list(object->signal_callbacks[signal_code],
                                        signal_callback);
   if (lst)
   {
      eina_signal_callback_del(lst->data);
      object->signal_callbacks[signal_code] =
         eina_list_remove_list(object->signal_callbacks[signal_code], lst);
   }
}

/**
 * @internal
 * @brief Gets the signal-callbacks connected to the signal @a signal of the
 *        object @a object
 *
 * @param object the object connected to the signal
 * @param signal_code the signal identification code
 * @param callbacks the location of a list where the signal-callbacks will be
 *                  returned. You should not free this list.
 *
 * @note You usually do not need to call this function manually, it is used
 *       by eina_signal_emit().
 */
void eobj_object_signal_callbacks_get(Eobj_Object *object, int signal_code,
                                     Eina_List **callbacks)
{
   if (!callbacks)
      return;

   *callbacks = object->signal_callbacks[signal_code];
}

/**
 * @brief Adds a weak-pointer to the object. A weak-pointer is a pointer that will be automatically set
 * to NULL when the object is destroyed
 * @param object an object
 * @param pointer_location the location of the weak-pointer
 * @warning if the @a pointer_location is not accessible when the object is destroyed, it may segfaults. So you have
 * to use eobj_object_weak_pointer_remove() when @a pointer_location becomes inaccessible
 * @see eobj_object_weak_pointer_remove()
 */
void eobj_object_weak_pointer_add(Eobj_Object *object, void **pointer_location)
{
   if (!object || !pointer_location || object->destroy_me)
      return;
   if (eina_list_find(object->weak_pointers, pointer_location))
      return;

   object->weak_pointers = eina_list_append(object->weak_pointers, pointer_location);
}

/**
 * @brief Removes a weak-pointer from the object
 * @param object an object
 * @param pointer_location the location of the weak-pointer to remove
 * @see eobj_object_weak_pointer_add()
 */
void eobj_object_weak_pointer_remove(Eobj_Object *object, void **pointer_location)
{
   if (!object || !pointer_location)
      return;
   object->weak_pointers = eina_list_remove(object->weak_pointers, pointer_location);
}

/**
 * @brief Associates a data pointer to a key. The pointer will be carried by the object and could be retrieved
 * later with eobj_object_data_get(). If the object already has an association for the given key, the old data
 * will be destroyed
 * @param object the object which the data will be added to
 * @param key the key to associate to the data
 * @param value the value of the data
 * @note If you want the data to be freed when the object is destroyed or when the value is changed,
 * use eobj_object_data_set_full() instead.
 */
void eobj_object_data_set(Eobj_Object *object, const char *key, void *value)
{
   eobj_object_data_set_full(object, key, value, NULL);
}

/**
 * @brief Associates a data pointer to a key. The pointer will be carried by the object and could be retrieved
 * later with eobj_object_data_get(). If the object already has an association for the given key, the old data
 * will be destroyed
 * @param object the object which the data will be added to
 * @param key the key to associate to the data
 * @param value the value of the data
 * @param free_cb the function to call on @a value when the object is destroyed or when the value is changed
 */
void eobj_object_data_set_full(Eobj_Object *object, const char *key, void *value, void (*free_cb)(void *data))
{
   Eobj_Object_Data *data;

   if (!object || !key)
      return;

   if ((data = eina_hash_find(object->data_hash, key)))
   {
      if (data->free_cb)
         data->free_cb(data->value);
      object->data_hash = eina_hash_del(object->data_hash, key, NULL);
      free(data);
   }

   data = malloc(sizeof(Eobj_Object_Data));
   data->value = value;
   data->free_cb = free_cb;
   object->data_hash = eina_hash_add(object->data_hash, key, data);
}

/**
 * @brief Gets the pointer associated to the given key
 * @param object the object which has the data
 * @param key the key associated to the data
 * @return Returns the associated pointer, NULL on failure
 */
void *eobj_object_data_get(Eobj_Object *object, const char *key)
{
   Eobj_Object_Data *data;

   if (!object || !key || !(data = eina_hash_find(object->data_hash, key)))
      return NULL;
   return data->value;
}

/**
 * @brief Resets the default value of a property
 * @param object the object that has the property to reset
 * @param property_name the name of the property to reset
 */
void eobj_object_property_reset(Eobj_Object *object, const char *property_name)
{
   Eobj_Class *type;
   Eobj_Property *property;

   if (!object || !property_name)
      return;

   if (eina_type_property_find(object->type, property_name, &type, &property))
   {
      if (type->property_set)
         type->property_set(object, property->id, property->default_value);
   }
}

/**
 * @brief Sets the values of several properties
 * @param object the object that has the properties
 * @param first_property the name of the first property value
 * @param ... the value of the first property, followed by any number of property-name/property-value pairs,
 * terminated with NULL
 */
void eobj_object_properties_set(Eobj_Object *object, const char *first_property, ...)
{
   va_list args;

   if (!object || !first_property)
      return;

   va_start(args, first_property);
   eobj_object_properties_set_valist(object, first_property, args);
   va_end(args);
}

/**
 * @brief Sets the values of several properties
 * @param object the object that has the properties
 * @param first_property the name of the first property value
 * @param args the value of the first property, followed by any number of property-name/property-value pairs,
 * terminated with NULL
 */
void eobj_object_properties_set_valist(Eobj_Object *object, const char *first_property, va_list args)
{
   const char *property_name;
   Eobj_Class *type;
   Eobj_Property *property;
   Eobj_Property_Value *property_value;
   va_list args2;

   if (!object)
      return;

   va_copy(args2, args);
   for (property_name = first_property; property_name; property_name = va_arg(args2, const char *))
   {
      if (eina_type_property_find(object->type, property_name, &type, &property))
      {
         property_value = eina_property_value_create_valist(eina_property_type_get(property), &args2);
         if (type->property_set)
            type->property_set(object, property->id, property_value);
         eina_property_value_delete(property_value);
      }
      else
      {
         EOBJ_WARNING("The object %p of type \"%s\" has no property called \"%s\"",
           object, object->type->name, property_name);
         break;
      }
   }
   va_end(args2);
}

/**
 * @brief Gets the values of several properties
 * @param object the object that has the properties
 * @param first_property the name of the first property value
 * @param ... the value of the first property, followed by any number of property-name/property-value-location pairs,
 * terminated with NULL
 */
void eobj_object_properties_get(Eobj_Object *object, const char *first_property, ...)
{
   va_list args;

   if (!object || !first_property)
      return;

   va_start(args, first_property);
   eobj_object_properties_get_valist(object, first_property, args);
   va_end(args);
}

/**
 * @brief Gets the values of several properties
 * @param object the object that has the properties
 * @param first_property the name of the first property value
 * @param args the value of the first property, followed by any number of property-name/property-value-location pairs,
 * terminated with NULL
 */
void eobj_object_properties_get_valist(Eobj_Object *object, const char *first_property, va_list args)
{
   void *value_location;
   const char *property_name;
   Eobj_Class *type;
   Eobj_Property *property;
   Eobj_Property_Value *property_value;

   if (!object)
      return;

   property_value = eina_property_value_new();
   for (property_name = first_property; property_name; property_name = va_arg(args, const char *))
   {
      if (eina_type_property_find(object->type, property_name, &type, &property))
      {
         if (type->property_get)
         {
            type->property_get(object, property->id, property_value);

            value_location = va_arg(args, void *);
            eina_property_value_get(property_value, eina_property_value_type_get(property_value), value_location);
         }
      }
      else
      {
         EOBJ_WARNING("The object %p of type \"%s\" has no property called \"%s\"",
            object, object->type->name, property_name);
         break;
      }
   }
   eina_property_value_delete(property_value);
}

/**
 * @brief Calls the object's notification callbacks associated the given property.
 * It should be called each time the value of a property is changed
 * @param object an object
 * @param property_name the name of the property
 * @object_implementation
 */
void eobj_object_notify(Eobj_Object *object, const char *property_name)
{
   Eina_List *l;
   Eina_List **callbacks;
   Eobj_Notification_Callback *callback;

   if (!object || !property_name)
      return;
   if (!(callbacks = eina_hash_find(object->notification_callbacks, property_name)))
      return;

   object->notifying++;

   for (l = *callbacks; l; l = l->next)
   {
      callback = l->data;
      if (callback->callback && !callback->delete_me)
         callback->callback(object, property_name, callback->data);
   }

   object->notifying--;
   if (object->notifying <= 0 && object->should_delete_cbs)
   {
      eina_hash_foreach(object->notification_callbacks, _eobj_object_notification_callbacks_clean_cb, NULL);
      object->should_delete_cbs = EINA_FALSE;
   }
}

/**
 * @brief Adds a notification callback associated to a property of the object.
 * The callback will be called each time the value of the property is changed
 * (i.e. each time eobj_object_notify(object, property_name) is called).
 * @param object an object
 * @param property_name the name of the property
 * @param callback the callback function
 * @param data the data to pass to the callback
 */
void eobj_object_notification_callback_add(Eobj_Object *object, const char *property_name, void (*callback)(Eobj_Object *object, const char *property_name, void *data), void *data)
{
   Eina_List **list;
   Eobj_Notification_Callback *new_callback;

   if (!object || !property_name || !callback)
      return;

   if (!(list = eina_hash_find(object->notification_callbacks, property_name)))
   {
      list = malloc(sizeof(Eina_List *));
      *list = NULL;
      object->notification_callbacks = eina_hash_add(object->notification_callbacks, property_name, list);
   }

   new_callback = malloc(sizeof(Eobj_Notification_Callback));
   new_callback->callback = callback;
   new_callback->data = data;
   new_callback->delete_me = EINA_FALSE;
   *list = eina_list_append(*list, new_callback);
}

/**
 * @brief Removes a notification callback associated to a property of the object
 * @param object an object
 * @param property_name the name of the property
 * @param callback the callback function to remove
 */
void eobj_object_notification_callback_remove(Eobj_Object *object, const char *property_name, void (*callback)(Eobj_Object *object, const char *property_name, void *data))
{
   Eina_List *l, *next;
   Eina_List **list;
   Eobj_Notification_Callback *remove_callback;

   if (!object || !property_name || !callback)
      return;
   if (!(list = eina_hash_find(object->notification_callbacks, property_name)))
      return;

   for (l = *list; l; l = next)
   {
      remove_callback = l->data;
      next = l->next;
      if (remove_callback->callback == callback)
      {
         /* If the object is calling the notification-callbacks, we can't remove the callback from the list, or it may
          * segfault. So we remove it only if the object is not notifying. Otherwise, we just mark the callback
          * as "deleted" and we will actually remove it at the end of the notication process */
         if (object->notifying == 0)
         {
            free(remove_callback);
            *list = eina_list_remove_list(*list, l);
         }
         else
         {
            remove_callback->delete_me = EINA_TRUE;
            object->should_delete_cbs = EINA_TRUE;
         }
      }
   }
}



/**************************
 *
 * Documentation
 *
 **************************/

/**
 * @addtogroup Eobj_Object
 * Eobj_Object implements advanced features such as inheritance, constructors/destructors, signals and properties. @n @n
 *
 * <hr>
 * <b>Constructors/Destructors:</b> @n
 * A new object can be created with eobj_object_new(). For example:
 * @code
 * //Creates a new focusable slider, for the range [1.0 - 3.0] and with the initial value 2.0
 * slider = eobj_object_new(EOBJ_SLIDER_TYPE, "theme-group", "slider", "focusable", EINA_TRUE, "lower", 1.0, "upper", 3.0, "value", 2.0, NULL);
 * @endcode
 * The first argument is the type of the object to create, followed by any number of property-name/property-value pairs,
 * and terminated with NULL. @n
 * eobj_object_new() automatically calls the corresponding constructors of the object, from the constructor of
 * the base class to the constructor of the more derived class. @n
 *
 * You can then destroy the object with eobj_object_destroy(): it sets the weak-pointers of the object to NULL
 * (see eobj_object_weak_pointer_add()), emits the "destroyed" signal and queues the object for freeing. Thus, the
 * destructors will only be called at the beginning of the next main loop iteration (from the destructor of the more
 * derived class to the destructor of the ultimate base class). You should then not assume that eobj_object_destroy()
 * will directly call the destructors. @n @n
 *
 * <b>Signal concept:</b> @n
 * Each object has a list of signals that can be connected to one or several callbacks. The callbacks connected to
 * a signal will be automatically called when the signal is emitted with eina_signal_emit(). @n
 * You can connect a callback to a signal of an object with eina_signal_connect(). For example:
 * @code
 * //Callback prototype
 * void clicked_cb(Eina_Button *button, void *data);
 *
 * //Connects the callback "clicked_cb()" to the signal "clicked" of the button
 * eina_signal_connect("clicked", EOBJ_OBJECT(button), EOBJ_CALLBACK(clicked_cb), user_data);
 * @endcode
 *
 * You can also disconnect a callback from a signal of an object with eina_signal_disconnect(). For instance:
 * @code
 * //Disconnects the callback "clicked_cb()" from the signal "clicked"
 * eina_signal_disconnect("clicked", EOBJ_OBJECT(button), EOBJ_CALLBACK(clicked_cb));
 * @endcode
 *
 * Each object inherits the signals from its parent classes (for instance, an Eina_Button has the signals of Eobj_Object,
 * Eina_Widget, Eina_Container, Eina_Bin and Eina_Button).
 * Each object's documentation page has a list of the object's signals with the associated callback prototype and
 * a short explanation. @n
 * For more information about signals, see the documentation page of Eobj_Signal. @n @n
 *
 * <b>Property concept:</b> @n
 * Each object also has a list of properties. Each property has a specific type, a default value, and can be either
 * readable, writable or both. You can set or get the value of a property with eobj_object_properties_set() and
 * eobj_object_properties_set(). @n
 *
 * Another important point about the property system is that you can add a notification callback to a property. The
 * callback will be called each time the value of the property is changed (i.e. each time eobj_object_notify() is called
 * on that property). This can be done with eobj_object_notification_callback_add(). For example:
 * @code
 * //Notification callback prototype
 * void value_changed_cb(Eobj_Object *object, const char *property_name, void *data);
 *
 * //Adds a notification callback to the property "upper" of the slider.
 * //It will be called when the upper bound of the slider is changed
 * eobj_object_notification_callback_add(EOBJ_OBJECT(slider), "upper", value_changed_cb, user_data);
 * @endcode
 *
 * Each object inherits the properties from its parent classes (for instance, an Eina_Button has the properties of
 * Eobj_Object, Eina_Widget, Eina_Container, Eina_Bin and Eina_Button).
 * <hr>
 * @n @n
 *
 * \par Object Hierarchy:
 * - Eobj_Object
 *
 * \par Signals:
 * @signal_name "destroyed": Emitted when the object is destroyed, before all the destructors of the object are called.
 * Since the destructors have not been called yet, the object should still be usable at this point.
 * @signal_cb Eina_Bool callback(Eobj_Object *object, void *data)
 * @signal_arg object: the object which is about to be destroyed
 * @signal_data
 *
 * \par Properties:
 * @prop_name "name": The name of the object
 * @prop_type String (char *)
 * @prop_rw
 * @prop_val NULL
 */
