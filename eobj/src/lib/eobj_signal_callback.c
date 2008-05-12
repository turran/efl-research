#include "Eobj.h"
#include "eobj_private.h"
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * @internal
 * @brief Creates a new signal-callback for the signal @a signal, using the callback function @a callback, and
 * associated to user data @a data
 * @param callback the callback function to call when the signal is emitted
 * @param data the user data to pass to the callback function
 * @param swapped if @a swapped is EINA_TRUE, the callback function will be called with the data as the only argument
 * @return Returns the new signal-callback on success or NULL on failure
 * @warning The new signal-callback has to be freed with eobj_signal_callback_del()
 */
Eobj_Signal_Callback *eobj_signal_callback_new(Eobj_Callback callback, void *data, Eina_Bool swapped)
{
   Eobj_Signal_Callback *new_callback;

   if (!callback)
      return NULL;

   new_callback = malloc(sizeof(Eobj_Signal_Callback));
   new_callback->callback = callback;
   new_callback->data = data;
   new_callback->swapped = swapped;
   new_callback->blocked = EINA_FALSE;

   return new_callback;
}

/**
 * @internal
 * @brief Deletes the signal-callback
 * @param signal_callback the signal-callback to delete
 */
void eobj_signal_callback_del(Eobj_Signal_Callback *signal_callback)
{
   free(signal_callback);
}

/**
 * @internal
 * @brief Calls the callback @a callback on the object @a object
 *
 * @param signal signal of the signal-callback to call
 * @param callback the signal-callback to call
 * @param object the object to call the callback on
 * @param return_value the location for the return value (if none, it can be
 *        NULL)
 * @param args the arguments to pass to the callback
 * @note if the callback is blocked, it won't be called
 */
void eobj_signal_callback_call_valist(Eobj_Signal *signal,
   Eobj_Signal_Callback *callback, Eobj_Object *object, void *return_value,
   va_list args)
{
   Eobj_Marshaller marshaller;

   if (!callback || !callback->callback || callback->blocked ||
       !(marshaller = eobj_signal_marshaller_get(signal)))
      return;

   if (callback->swapped)
   {
      Eobj_Callback_Swapped swapped_callback = callback->callback;
      swapped_callback(callback->data);
   }
   else
   {
      va_list args2;

      va_copy(args2, args);
      marshaller(callback->callback, object, callback->data, return_value,
                 args2);
      va_end(args2);
   }
}

/**
 * @internal
 * @brief Prevents the callback from being called: eobj_signal_callback_call() will have no effect on the callback
 * @param callback the callback to block
 */
void eobj_signal_callback_block(Eobj_Signal_Callback *callback)
{
   if (!callback)
      return;
   callback->blocked = EINA_TRUE;
}

/**
 * @internal
 * @brief Unblocks the callback. It can no be called again by calling eobj_signal_callback_call()
 * @param callback the callback to unblock
 */
void eobj_signal_callback_unblock(Eobj_Signal_Callback *callback)
{
   if (!callback)
      return;
   callback->blocked = EINA_FALSE;
}

/**
 * @internal
 * @brief Gets whether or not the callback is blocked
 * @param callback a callback
 * @return Returns EINA_TRUE if the callback is blocked, EINA_FALSE otherwise
 */
Eina_Bool eobj_signal_callback_is_blocked(Eobj_Signal_Callback *callback)
{
   if (!callback)
      return EINA_FALSE;
   return callback->blocked;
}
