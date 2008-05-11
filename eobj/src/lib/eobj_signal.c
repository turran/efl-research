#include "Eobj.h"
#include "eobj_private.h"
/**
 * @addtogroup Eobj_Signal
 * @{
 */

static void _eobj_signal_free(Eobj_Signal *signal);

static Eina_List *_eobj_signal_signals_list = NULL;

/**************************
 *
 * Implementation
 *
 **************************/

/**
 * @internal
 * @brief Shutdowns the signal system: it destroys all the created signals
 */
void eobj_signal_shutdown(void)
{
   Eina_List *lst;

   for (lst = _eobj_signal_signals_list; lst; lst = lst->next)
      _eobj_signal_free(lst->data);

   eobj_list_free(_eobj_signal_signals_list);
}

/**
 * @internal
 * @brief Creates a new signal in the system.
 */
static Eobj_Signal *eobj_signal_new_raw(const char *signal_name, Eobj_Class *type,
   long handler_offset, Eobj_Marshaller marshaller)
{
   Eobj_Signal *new_signal;

   new_signal = malloc(sizeof(Eobj_Signal));
   new_signal->name = strdup(signal_name);
   new_signal->code = type->signals_count;
   new_signal->handler_offset = handler_offset;
   new_signal->marshaller = marshaller;

   type->signals_count++;

   _eobj_signal_signals_list = eobj_list_append(_eobj_signal_signals_list, new_signal);

   return new_signal;
}

/**
 * @brief Creates a new signal called @a signal_name, for the object type @a
 * type.
 *
 * @param signal_name the name of the new signal.
 * @param type the object type of the new signal.
 * @param handler_offset the offset of the default handler in the object's
 * struct (use EOBJ_MEMBER_OFFSET() to get it). -1 if there is no default
 * handler.
 * @param marshaller the marshaller of the signal: it will treat and pass the
 * arguments to the callbacks.
 *
 * @return Returns the new signal code, or -1 in case of failure
 */
int eobj_signal_new(const char *signal_name, Eobj_Class *type, long handler_offset,
                   Eobj_Marshaller marshaller)
{
   Eobj_Signal *new_signal;

   if (!signal_name || !type || !marshaller)
      return -1;

   new_signal = eobj_signal_new_raw(signal_name, type, handler_offset,
                                   marshaller);

   if (!new_signal)
      return -1;

   type->signals =
      (Eobj_Signal **) realloc(type->signals,
                              type->signals_count * sizeof(Eobj_Signal *));

   type->signals[new_signal->code] = new_signal;

   return new_signal->code;
}

/**
 * @internal
 * @brief Creates a new signal called @a signal_name, for the object type @a
 * type, based on an @a Eobj_Signal_Description.
 */
void eobj_signal_new_with_desc(Eobj_Class *type,
                              const Eobj_Signal_Description *desc)
{
   Eobj_Signal *new_signal;

   if (!desc || !desc->signal_code_store)
      return;

   if (!type || !desc->name || !desc->marshaller)
   {
      *(desc->signal_code_store) = -1;
      return;
   }

   new_signal = eobj_signal_new_raw(desc->name, type, desc->handler_offset,
                                   desc->marshaller);

   if (!new_signal)
   {
      *(desc->signal_code_store) = -1;
      return;
   }

   type->signals[new_signal->code] = new_signal;
   *(desc->signal_code_store) = new_signal->code;
}

/**
 * @brief Gets the signal corresponding to the name and the object type.
 *
 * assumes @a signal_name and @a type to be valid.
 */
static Eobj_Signal *eobj_signal_lookup_by_name(const char *signal_name, Eobj_Class *type)
{
   unsigned i;

   if (!type->signals)
      return NULL;

   for (i = 0; i < type->signals_count; i++)
      if (strcmp(type->signals[i]->name, signal_name) == 0)
         return type->signals[i];

   return NULL;
}

/**
 * @brief Gets the signal code corresponding to the name and the object type.
 * @internal
 * assumes @a signal_name and @a type to be valid.
 */
static int eobj_signal_lookup_code(const char *signal_name, Eobj_Class *type)
{
   Eobj_Signal *signal;

   signal = eobj_signal_lookup_by_name(signal_name, type);
   if (signal)
     return signal->code;
   else
     return -1;
}


/**
 * @brief Gets the name of the signal
 * @param signal a signal
 * @return Returns the name of the signal, or NULL on failure
 */
const char *eobj_signal_name_get(Eobj_Signal *signal)
{
   return signal ? signal->name : NULL;
}

/**
 * @brief Connects a callback to a signal of the object @a object.
 *
 * When the signal of the object will be emitted, this callback will be
 * automatically called.
 *
 * @param signal_code the signal code to connect to the callback
 * @param object the object to connect to the callback
 * @param callback the callback to call when the signal is emitted. This
 *        callback should return Eina_Bool with EINA_TRUE to continue
 *        and EINA_FALSE to stop signal propagation to next callbacks
 *        during the current emission.
 * @param data the data to pass to the callback
 * @param swapped if @a swapped == EINA_TRUE, the callback will be called
 *        with @a data as the only argument. It can be useful to set it to
 *        EINA_TRUE if you just want to call one function on an object when the
 *        signal is emitted.
 * @param after if @a after == EINA_TRUE, the callback will be called after all
 *        the callbacks already connected to this signal. Otherwise, it will be
 *        called before all of them (default behavior)
 *
 * @return Returns a pointer identifying the connected callback, which can be
 * used later for disconnecting only this specific signal.
 */
const Eobj_Signal_Callback *eobj_signal_connect_full_by_code(int signal_code, Eobj_Object *object, Eobj_Callback callback, void *data, Eina_Bool swapped, Eina_Bool after)
{
   Eobj_Signal_Callback *new_callback;
   if (!object || !callback)
      return NULL;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal connection: the object type \"%s\" doesn't "
                  "have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return NULL;
   }

   if (!(new_callback = eobj_signal_callback_new(callback, data, swapped)))
      return NULL;

   eobj_object_signal_callback_add(object, signal_code, new_callback, after);
   return new_callback;
}

/**
 * @brief Connects a callback to a signal of the object @a object.
 *
 * When the signal of the object will be emitted, this callback will be
 * automatically called.
 *
 * @param signal_name the signal name to connect to the callback
 * @param object the object to connect to the callback
 * @param callback the callback to call when the signal is emitted
 * @param data the data to pass to the callback
 * @param swapped if @a swapped == EINA_TRUE, the callback will be called
 *        with @a data as the only argument. It can be useful to set it to
 *        EINA_TRUE if you just want to call one function on an object when
 *        the signal is emitted.
 * @param after if @a after == EINA_TRUE, the callback will be called after
 *        all the callbacks already connected to this signal. Otherwise, it
 *        will be called before all of them (default behavior)
 *
 * @return Returns a pointer identifying the connected callback, which can be
 * used later for disconnecting only this specific signal.
 */
const Eobj_Signal_Callback *eobj_signal_connect_full_by_name(const char *signal_name, Eobj_Object *object, Eobj_Callback callback, void *data, Eina_Bool swapped, Eina_Bool after)
{
   int signal_code;

   if (!object || !signal_name)
      return EINA_FALSE;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   return eobj_signal_connect_full_by_code(signal_code, object, callback, data,
                                          swapped, after);
}

inline const Eobj_Signal_Callback *eobj_signal_connect(const char *signal_name,
   Eobj_Object *object, Eobj_Callback callback, void *data)
{
   return eobj_signal_connect_full_by_name(signal_name, object, callback, data,
                                          EINA_FALSE, EOBJ_FALSE);
}

inline const Eobj_Signal_Callback *eobj_signal_connect_after(
   const char *signal_name, Eobj_Object *object, Eobj_Callback callback,
   void *data)
{
   return eobj_signal_connect_full_by_name(signal_name, object, callback, data,
                                          EINA_FALSE, EINA_TRUE);
}

inline const Eobj_Signal_Callback *eobj_signal_connect_swapped(
   const char *signal_name, Eobj_Object *object, Eobj_Callback callback,
   void *data)
{
   return eobj_signal_connect_full_by_name(signal_name, object, callback, data,
                                          EINA_TRUE, EINA_FALSE);
}

inline const Eobj_Signal_Callback *eobj_signal_connect_by_code(int signal_code,
   Eobj_Object *object, Eobj_Callback callback, void *data)
{
   return eobj_signal_connect_full_by_code(signal_code, object, callback, data,
                                          EINA_FALSE, EOBJ_FALSE);
}

inline const Eobj_Signal_Callback *eobj_signal_connect_after_by_code(
   int signal_code, Eobj_Object *object, Eobj_Callback callback, void *data)
{
   return eobj_signal_connect_full_by_code(signal_code, object, callback, data,
                                          EINA_FALSE, EINA_TRUE);
}

inline const Eobj_Signal_Callback *eobj_signal_connect_swapped_by_code(
   int signal_code, Eobj_Object *object, Eobj_Callback callback, void *data)
{
   return eobj_signal_connect_full_by_code(signal_code, object, callback, data,
                                          EINA_TRUE, EINA_FALSE);
}

void eobj_signal_connect_multiple(Eobj_Signal_Connect_Desc *desc,
                                 Eobj_Object *object, void *data)
{
   Eobj_Signal_Connect_Desc *s = desc;

   if (!object || !s)
      return;

   for (; s->signal_code >= 0; s++)
      s->scb = eobj_signal_connect_by_code(s->signal_code, object, s->callback,
                                          data);
}

void eobj_signal_disconnect_multiple(Eobj_Signal_Connect_Desc *desc,
                                    Eobj_Object *object)
{
   Eobj_Signal_Connect_Desc *s = desc;

   if (!object || !s)
      return;

   for (; s->signal_code >= 0; s++)
      eobj_signal_disconnect_scb_by_code(s->signal_code, object, s->scb);
}


/**
 * @brief Disconnects a callback from a signal, the callback won't be called anymore when the signal is emitted.
 * @param signal_name the name of the signal connected to the callback to disconnect
 * @param object the object connected to the callback to disconnect
 * @param callback the callback to disconnect
 * @param data passed to the callback when connected
 */
void eobj_signal_disconnect_by_code(int signal_code, Eobj_Object *object,
                                   Eobj_Callback callback, void *data)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !callback)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal disconnection: the object type \"%s\" "
                  "doesn't have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback->callback == callback &&
          signal_callback->data == data)
      {
         eobj_object_signal_callback_remove(object, signal_code,
                                           signal_callback);
         break;
      }
   }
}

/**
 * @brief Disconnects a callback from a signal, the callback won't be called
 * anymore when the signal is emitted. Removes one specific occurence of a
 * callback function for a signal, identified by the value returned in
 * connect() calls.
 * @param signal_code code of the signal to disconnect
 * @param object the object connected to the callback to disconnect
 * @param scb the unique signal callback to disconnect
 */
void eobj_signal_disconnect_scb_by_code(int signal_code, Eobj_Object *object,
                                       const Eobj_Signal_Callback *scb)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !scb)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal disconnection: the object type \"%s\" "
                  "doesn't have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback == scb)
      {
         eobj_object_signal_callback_remove(object, signal_code,
                                           signal_callback);
         break;
      }
   }
}

void eobj_signal_disconnect(const char *signal_name, Eobj_Object *object,
                           Eobj_Callback callback, void *data)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   eobj_signal_disconnect_by_code(signal_code, object, callback, data);
}

void eobj_signal_disconnect_scb(const char *signal_name, Eobj_Object *object,
                               const Eobj_Signal_Callback *scb)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   eobj_signal_disconnect_scb_by_code(signal_code, object, scb);
}

/**
 * @brief Disconnects all callbacks from a signal
 * @param signal_code the code of the signal for which all callbacks will be disconnected
 * @param object the object for which all callbacks will be disconnected
 */
void eobj_signal_disconnect_all_by_code(int signal_code, Eobj_Object *object)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal disconnection: the object type \"%s\" "
                  "doesn't have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;
      eobj_object_signal_callback_remove(object, signal_code, signal_callback);
   }
}

/**
 * @brief Disconnects all callbacks from a signal
 * @param signal_name the name of the signal for which all callbacks will be disconnected
 * @param object the object for which all callbacks will be disconnected
 */
void eobj_signal_disconnect_all(const char *signal_name, Eobj_Object *object)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   eobj_signal_disconnect_all_by_code(signal_code, object);
}

/**
 * @brief Blocks a callback from being called when the corresponding signal is
 * emitted. Unlike eobj_signal_disconnect(), the callback is not removed, and
 * can be easily unblock with eobj_signal_unblock().
 *
 * @param signal_code the code of the signal connected to the callback to block
 * @param object the object connected to the callback to block
 * @param callback the callback function to block
 * @param data passed to the callback when connected
 */
void eobj_signal_block_by_code(int signal_code, Eobj_Object *object,
                              Eobj_Callback callback, void *data)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !callback)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal block: the object type \"%s\" doesn't "
                  "have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback->callback == callback &&
          signal_callback->data == data)
      {
         eobj_signal_callback_block(signal_callback);
         break;
      }
   }
}

/**
 * @brief Blocks a callback from being called when the corresponding signal is
 * emitted. Unlike eobj_signal_disconnect(), the callback is not removed, and
 * can be easily unblock with eobj_signal_unblock().
 *
 * @param signal_name the name of the signal connected to the callback to block
 * @param object the object connected to the callback to block
 * @param callback the callback function to block
 * @param data passed to the callback when connected
 */
void eobj_signal_block(const char *signal_name, Eobj_Object *object,
                      Eobj_Callback callback, void *data)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   eobj_signal_block_by_code(signal_code, object, callback, data);
}

/**
 * @brief Blocks a callback from being called when the corresponding signal is
 * emitted. Unlike eobj_signal_disconnect(), the callback is not removed, and
 * can be easily unblock with eobj_signal_unblock().
 *
 * @param signal_code code of the signal connected to the callback to block
 * @param object the object connected to the callback to block
 * @param scb the unique signal callback to block
 */
void eobj_signal_block_scb_by_code(int signal_code, Eobj_Object *object,
                                  const Eobj_Signal_Callback *scb)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !scb)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal block: the object type \"%s\" doesn't "
                  "have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback == scb)
      {
         eobj_signal_callback_block(signal_callback);
         break;
      }
   }
}

/**
 * @brief Blocks a callback from being called when the corresponding signal is
 * emitted.
 *
 * Unlike eobj_signal_disconnect(), the callback is not removed, and can be
 * easily unblock with eobj_signal_unblock().
 *
 * @param signal_name the name of the signal connected to the callback to block
 * @param object the object connected to the callback to block
 * @param scb the unique signal callback to block
 */
void eobj_signal_block_scb(const char *signal_name, Eobj_Object *object,
                          const Eobj_Signal_Callback *scb)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));

   eobj_signal_block_scb_by_code(signal_code, object, scb);
}

void eobj_signal_unblock_by_code(int signal_code, Eobj_Object *object,
                                Eobj_Callback callback, void *data)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !callback)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal unblock: the object type \"%s\" doesn't "
                  "have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback->callback == callback && signal_callback->data == data)
      {
         eobj_signal_callback_unblock(signal_callback);
         break;
      }
   }
}

/**
 * @brief Unblocks a blocked callback.
 *
 * The callback will no longer be prevented from being called when the
 * corresponding signal is emitted.
 *
 * @param signal_name name of the signal connected to the callback to unblock
 * @param object the object connected to the callback to unblock
 * @param callback the callback function to unblock
 * @param data passed to the callback when connected
 */
void eobj_signal_unblock(const char *signal_name, Eobj_Object *object,
                        Eobj_Callback callback, void *data)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));
   eobj_signal_unblock_by_code(signal_code, object, callback, data);
}



/**
 * @brief Unblocks a blocked callback.
 *
 * See eobj_signal_unblock().
 *
 * @param signal_code code of the signal connected to the callback to unblock
 * @param object the object connected to the callback to unblock
 * @param scb the unique signal callback to unblock
 */
void eobj_signal_unblock_scb_by_code(int signal_code, Eobj_Object *object,
                                    const Eobj_Signal_Callback *scb)
{
   Eina_List *c;
   Eobj_Signal_Callback *signal_callback;

   if (!object || !scb)
      return;

   if (signal_code < 0 || signal_code >= object->type->signals_count)
   {
      EOBJ_WARNING("Invalid signal unblock: the object type \"%s\" doesn't "
                  "have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return;
   }

   c = NULL;
   eobj_object_signal_callbacks_get(object, signal_code, &c);
   while (c)
   {
      signal_callback = c->data;
      c = c->next;

      if (signal_callback == scb)
      {
         eobj_signal_callback_unblock(signal_callback);
         break;
      }
   }
}

/**
 * @brief Unblocks a blocked callback.
 *
 * See eobj_signal_unblock().
 *
 * @param signal_name name of the signal connected to the callback to unblock
 * @param object the object connected to the callback to unblock
 * @param scb the unique signal callback to unblock
 */
void eobj_signal_unblock_scb(const char *signal_name, Eobj_Object *object,
                            const Eobj_Signal_Callback *scb)
{
   int signal_code;

   if (!object || !signal_name)
      return;

   signal_code = eobj_signal_lookup_code(signal_name,
                                        eobj_object_object_type_get(object));
   eobj_signal_unblock_scb_by_code(signal_code, object, scb);
}


/**
 * @brief Emits the signal: it will call the callbacks connected to the
 * signal @a signal.
 *
 * @param signal_code code of the signal to emit
 * @param object the object which will emit the signal
 * @param ... the arguments to pass to the callback function
 *
 * @return Returns EINA_FALSE if the signal has been stopped
 *         and EINA_TRUE otherwise
 */
Eina_Bool eobj_signal_emit(int signal_code, Eobj_Object *object, ...)
{
   va_list args;
   Eobj_Signal *signal;
   Eina_Bool ret;

   if (!object)
      return EINA_FALSE;

   if (signal_code < 0 || signal_code >= object->type->signals_count ||
       !(signal = object->type->signals[signal_code]))
   {
      EOBJ_WARNING("Invalid signal emission: the object type \"%s\" "
                  "doesn't have a signal with code \"%d\"", object->type->name,
                  signal_code);
      return EINA_FALSE;
   }

   va_start(args, object);
   ret = eobj_signal_emit_valist(signal, object, args);
   va_end(args);

   return ret;
}

/**
 * @brief Emits the signal: it will call the callbacks connected to the
 *        signal @a signal
 *
 * @param signal the name of the signal to emit
 * @param object the object which will emit the signal
 * @param ... the arguments to pass to the callback function
 *
 * @return Returns EINA_FALSE if the signal has been stopped
 *         and EINA_TRUE otherwise
 */
Eina_Bool eobj_signal_emit_by_name(const char *signal_name, Eobj_Object *object,
                                 ...)
{
   va_list args;
   Eobj_Signal *signal;
   Eina_Bool ret;

   if (!object || !signal_name)
      return EINA_FALSE;

   signal = eobj_signal_lookup_by_name(signal_name, eobj_object_object_type_get(object));
   if (!signal)
   {
      EOBJ_WARNING("Invalid signal emission: the object type doesn't have "
                  "a signal called \"%s\"", signal_name);
      return EINA_FALSE;
   }

   va_start(args, object);
   ret = eobj_signal_emit_valist(signal, object, args);
   va_end(args);

   return ret;
}

/**
 * @brief Emits the signal: it will call the callbacks connected to the
 *        signal @a signal
 *
 * @param signal the signal to emit
 * @param object the object which will emit the signal
 * @param args the arguments to pass to the callback function
 *
 * @return Returns @a object, or NULL if @a object has been destroyed by
 *         one of the callbacks
 * @return Returns EINA_FALSE if the signal has been stopped
 *         and EINA_TRUE otherwise
 */
Eina_Bool eobj_signal_emit_valist(Eobj_Signal *signal, Eobj_Object *object,
                                va_list args)
{
   Eina_List *callbacks;
   Eobj_Signal_Callback *callback;
   Eina_Bool keep_emission = EINA_TRUE;
   va_list args2;
   void *object_ptr;

   if (!object || !signal)
      return EINA_FALSE;

   /* The pointer object will be set to NULL if the object is destroyed
    * by a callback */
   object_ptr = object;
   eobj_object_weak_pointer_add(object, &object_ptr);

   /* Calls the default handler */
   if (signal->handler_offset >= 0 && signal->marshaller)
   {
      Eobj_Callback *default_handler;

      default_handler = (void *)object + signal->handler_offset;
      if (*default_handler)
      {
         va_copy(args2, args);
         signal->marshaller(*default_handler, object, NULL, &keep_emission,
                            args2);
         va_end(args2);
      }
   }

   /* Then we call the corresponding callbacks */
   if (object_ptr && keep_emission)
   {
      callbacks = NULL;
      eobj_object_signal_callbacks_get(object, signal->code, &callbacks);
      while (keep_emission && callbacks && object_ptr)
      {
         callback = callbacks->data;
         callbacks = callbacks->next;

         va_copy(args2, args);
         eobj_signal_callback_call_valist(signal, callback, object,
                                         &keep_emission, args2);
         va_end(args2);
      }
   }

   if (object_ptr)
      eobj_object_weak_pointer_remove(object, &object_ptr);

   return keep_emission;
}

/**
 * @brief Gets the marshaller used by the signal
 * @param signal a signal
 * @return Returns the marshaller used by the signal or NULL on failure
 */
Eobj_Marshaller eobj_signal_marshaller_get(Eobj_Signal *signal)
{
   if (!signal)
      return NULL;
   return signal->marshaller;
}

/**
 * @brief Gets a list of all the current signals
 * @return Returns an Eina_List containing all the signals.
 */
Eina_List * eobj_signal_get_all()
{
   return _eobj_signal_signals_list;
}

/**************************
 *
 * Private functions
 *
 **************************/

/* Frees the signal */
static void _eobj_signal_free(Eobj_Signal *signal)
{
   if (!signal)
      return;

   free(signal->name);
   free(signal);
}

/** @} */

/**************************
 *
 * Documentation
 *
 **************************/

/**
 * @addtogroup Eobj_Signal
 *
 * TODO: write doc for Eobj_Signal!!
 */
