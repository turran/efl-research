#ifndef EOBJ_MARSHALLERS_H_
#define EOBJ_MARSHALLERS_H_

#ifdef __cplusplus
extern "C" {
#endif

void eobj_marshaller_VOID(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_INT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_INT_INT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_DOUBLE(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_OBJECT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_POINTER_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);
void eobj_marshaller_INT_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments);

#ifdef __cplusplus
}
#endif

#endif
