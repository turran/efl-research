/** @file eina_signal_callback.h */
#ifndef _EOBJ_SIGNAL_CALLBACK_H_
#define _EOBJ_SIGNAL_CALLBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup Eobj_Signal_Callback Eina_Signal_Callback
 * @brief For more info, see the page about Eobj_Signal
 * @{
 */

/** Casts the function pointer to an Eobj_Signal_Callback_Function */
#define EOBJ_CALLBACK(callback)    ((Eina_Callback)(callback))

/**
 * @brief A callback that will be called when the corresponding signal is emitted
 * @structinfo
 */
struct _Eobj_Signal_Callback
{
   /* private: */
   Eobj_Callback callback;
   void *data;
   Eina_Bool swapped:1;
   Eina_Bool blocked:1;
};


Eobj_Signal_Callback *eina_signal_callback_new(Eobj_Callback callback, void *data, Eina_Bool swapped);
void     eina_signal_callback_del(Eobj_Signal_Callback *signal_callback);

void     eina_signal_callback_call_valist(Eobj_Signal *signal,
            Eobj_Signal_Callback *callback, Eobj_Object *object,
            void *return_value, va_list args);
void     eina_signal_callback_block(Eobj_Signal_Callback *callback);
void     eina_signal_callback_unblock(Eobj_Signal_Callback *callback);
Eina_Bool eina_signal_callback_is_blocked(Eobj_Signal_Callback *callback);


/** @} */

#ifdef __cplusplus
}
#endif

#endif
