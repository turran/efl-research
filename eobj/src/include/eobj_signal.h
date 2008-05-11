/** @file eobj_signal.h */
#ifndef EOBJ_SIGNAL_H_
#define EOBJ_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup Eobj_Signal Eobj_Signal
 * @brief The signal system is a flexible system to create, connect and emit
 * signals
 * @{
 */

typedef struct Eobj_Signal_Connect_Desc {
   const Eobj_Signal_Callback *scb;
   int signal_code;
   Eobj_Callback callback;
} Eobj_Signal_Connect_Desc;

#define EOBJ_SC_DESC_SENTINEL {NULL, -1, NULL}
#define EOBJ_SC_DESC(code, callback) \
   { NULL, (code), EOBJ_CALLBACK(callback) }

/** Gets the offset of a member of a struct (used when you create a new signal
 * with a default handler, with eobj_signal_new()) */
#define EOBJ_MEMBER_OFFSET(type, member)   ((long)((long *)&((type *)0)->member))

typedef struct Eobj_Signal_Description
{
  int *signal_code_store; /**< where to store signal code. */
  const char *name; /**< signal name */
  long handler_offset; /**<  */
  Eobj_Marshaller marshaller;
} Eobj_Signal_Description;

#define EOBJ_SIGNAL_DESCRIPTION_SENTINEL {NULL, NULL, 0, NULL}

#define EOBJ_SIGNAL_DESC_NO_HANDLER(signal_code, name, marshaller) \
        {&(signal_code), (name), -1, \
           (marshaller)}

#define EOBJ_SIGNAL_DESC_HANDLER(signal_code, name, type, handler, marshaller) \
        {&(signal_code), (name), EOBJ_MEMBER_OFFSET(type, handler), \
           (marshaller)}

/**
 * @brief The structure of a signal
 * @structinfo
 */
struct _Eobj_Signal
{
   /* private: */
   char *name;
   int code;
   long handler_offset;
   Eobj_Marshaller marshaller;
};

void eobj_signal_shutdown(void);

int         eobj_signal_new(const char *signal_name, Eobj_Class *type,
                           long handler_offset, Eobj_Marshaller marshaller);
void eobj_signal_new_with_desc(Eobj_Class *type,
                              const Eobj_Signal_Description *desc);
void        eobj_signal_delete(Eobj_Signal *signal);

const char    *eobj_signal_name_get(Eobj_Signal *signal);
Eobj_Marshaller eobj_signal_marshaller_get(Eobj_Signal *signal);

const Eobj_Signal_Callback *eobj_signal_connect_full_by_code(int signal_code,
   Eobj_Object *object, Eobj_Callback callback, void *data, Eina_Bool swapped,
   Eina_Bool after);
const Eobj_Signal_Callback *eobj_signal_connect_full_by_name(
   const char *signal_name, Eobj_Object *object, Eobj_Callback callback,
   void *data, Eina_Bool swapped, Eina_Bool after);

inline const Eobj_Signal_Callback *eobj_signal_connect_by_code(int signal_code,
   Eobj_Object *object, Eobj_Callback callback, void *data);
inline const Eobj_Signal_Callback *eobj_signal_connect(const char *signal_name,
   Eobj_Object *object, Eobj_Callback callback, void *data);
inline const Eobj_Signal_Callback *eobj_signal_connect_after_by_code(
   int signal_code, Eobj_Object *object, Eobj_Callback callback, void *data);
inline const Eobj_Signal_Callback *eobj_signal_connect_after(
   const char *signal_name, Eobj_Object *object, Eobj_Callback callback,
   void *data);
inline const Eobj_Signal_Callback *eobj_signal_connect_swapped_by_code(
   int signal_code, Eobj_Object *object, Eobj_Callback callback, void *data);
inline const Eobj_Signal_Callback *eobj_signal_connect_swapped(
   const char *signal_name, Eobj_Object *object, Eobj_Callback callback,
   void *data);

void eobj_signal_connect_multiple(Eobj_Signal_Connect_Desc *desc,
                                 Eobj_Object *object, void *data);

void eobj_signal_disconnect_by_code(int signal_code, Eobj_Object *object,
                                   Eobj_Callback callback, void *data);
void eobj_signal_disconnect(const char *signal_name, Eobj_Object *object,
                           Eobj_Callback callback, void *data);
void eobj_signal_disconnect_scb_by_code(int signal_code, Eobj_Object *object,
                                       const Eobj_Signal_Callback *scb);
void eobj_signal_disconnect_scb(const char *signal_name, Eobj_Object *object,
                               const Eobj_Signal_Callback *scb);
void eobj_signal_disconnect_all_by_code(int signal_code, Eobj_Object *object);
void eobj_signal_disconnect_all(const char *signal_name, Eobj_Object *object);

void eobj_signal_disconnect_multiple(Eobj_Signal_Connect_Desc *desc,
                                    Eobj_Object *object);

void eobj_signal_block_by_code(int signal_code, Eobj_Object *object,
                              Eobj_Callback callback, void *data);
void eobj_signal_block(const char *signal_name, Eobj_Object *object,
                      Eobj_Callback callback, void *data);
void eobj_signal_block_scb_by_code(int signal_code, Eobj_Object *object,
                                  const Eobj_Signal_Callback *scb);
void eobj_signal_block_scb(const char *signal_name, Eobj_Object *object,
                          const Eobj_Signal_Callback *scb);

void eobj_signal_unblock_by_code(int signal_code, Eobj_Object *object,
                                Eobj_Callback callback, void *data);
void eobj_signal_unblock(const char *signal_name, Eobj_Object *object,
                        Eobj_Callback callback, void *data);
void eobj_signal_unblock_scb_by_code(int signal_code, Eobj_Object *object,
                                    const Eobj_Signal_Callback *scb);
void eobj_signal_unblock_scb(const char *signal_name, Eobj_Object *object,
                            const Eobj_Signal_Callback *scb);

Eina_Bool eobj_signal_emit(int signal_code, Eobj_Object *object, ...);
Eina_Bool eobj_signal_emit_by_name(const char *signal_name, Eobj_Object *object,
                                 ...);
Eina_Bool eobj_signal_emit_valist(Eobj_Signal *signal, Eobj_Object *object,
                                va_list args);

Eina_List * eobj_signal_get_all();
const Eobj_Class * eobj_signal_object_type_get(Eobj_Signal *signal);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
