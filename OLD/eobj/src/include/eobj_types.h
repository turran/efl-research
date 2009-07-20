#ifndef EOBJ_TYPES_H_
#define EOBJ_TYPES_H_

typedef struct _Eobj_Object Eobj_Object;
typedef struct _Eobj_Class Eobj_Class;
typedef struct _Eobj_Signal Eobj_Signal;
typedef struct _Eobj_Signal_Callback Eobj_Signal_Callback;
typedef struct _Eobj_Property Eobj_Property;
typedef struct _Eobj_Property_Value Eobj_Property_Value;
typedef struct _Eobj_Notification_Callback Eobj_Notification_Callback;

typedef Eina_Bool (*Eobj_Callback)();
typedef Eina_Bool (*Eobj_Callback_Swapped)(void *data);
typedef void (*Eobj_Marshaller)(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);


#endif /*EOBJ_TYPES_H_*/
