#include "Eobj.h"

/* eobj_marshaller_VOID */
void eobj_marshaller_VOID(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_VOID)(Eobj_Object *object, void *data);
   Eobj_Callback_VOID callback_VOID;
   Eina_Bool result;

   callback_VOID = (Eobj_Callback_VOID)callback;
   result = callback_VOID(object, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_INT */
void eobj_marshaller_INT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_INT)(Eobj_Object *object, int arg1, void *data);
   Eobj_Callback_INT callback_INT;
   int arg1;
   Eina_Bool result;

   arg1 = va_arg(arguments, int);

   callback_INT = (Eobj_Callback_INT)callback;
   result = callback_INT(object, arg1, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_INT_INT */
void eobj_marshaller_INT_INT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_INT_INT)(Eobj_Object *object, int arg1, int arg2, void *data);
   Eobj_Callback_INT_INT callback_INT_INT;
   int arg1;
   int arg2;
   Eina_Bool result;

   arg1 = va_arg(arguments, int);
   arg2 = va_arg(arguments, int);

   callback_INT_INT = (Eobj_Callback_INT_INT)callback;
   result = callback_INT_INT(object, arg1, arg2, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_DOUBLE */
void eobj_marshaller_DOUBLE(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_DOUBLE)(Eobj_Object *object, double arg1, void *data);
   Eobj_Callback_DOUBLE callback_DOUBLE;
   double arg1;
   Eina_Bool result;

   arg1 = va_arg(arguments, double);

   callback_DOUBLE = (Eobj_Callback_DOUBLE)callback;
   result = callback_DOUBLE(object, arg1, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_OBJECT */
void eobj_marshaller_OBJECT(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_OBJECT)(Eobj_Object *object, Eobj_Object *arg1, void *data);
   Eobj_Callback_OBJECT callback_OBJECT;
   Eobj_Object *arg1;
   Eina_Bool result;

   arg1 = va_arg(arguments, Eobj_Object *);

   callback_OBJECT = (Eobj_Callback_OBJECT)callback;
   result = callback_OBJECT(object, arg1, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_POINTER */
void eobj_marshaller_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_POINTER)(Eobj_Object *object, void *arg1, void *data);
   Eobj_Callback_POINTER callback_POINTER;
   void *arg1;
   Eina_Bool result;

   arg1 = va_arg(arguments, void *);

   callback_POINTER = (Eobj_Callback_POINTER)callback;
   result = callback_POINTER(object, arg1, data);

   *((Eina_Bool *)return_value) = result;
}


/* eobj_marshaller_POINTER_POINTER */
void eobj_marshaller_POINTER_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_POINTER_POINTER)(Eobj_Object *object, void *arg1, void *arg2, void *data);
   Eobj_Callback_POINTER_POINTER callback_POINTER_POINTER;
   void *arg1;
   void *arg2;
   Eina_Bool result;

   arg1 = va_arg(arguments, void *);
   arg2 = va_arg(arguments, void *);

   callback_POINTER_POINTER = (Eobj_Callback_POINTER_POINTER)callback;
   result = callback_POINTER_POINTER(object, arg1, arg2, data);

   *((Eina_Bool *)return_value) = result;
}

/* eobj_marshaller_INT_POINTER */
void eobj_marshaller_INT_POINTER(Eobj_Callback callback, Eobj_Object *object, void *data, void *return_value, va_list arguments)
{
   typedef Eina_Bool (*Eobj_Callback_INT_POINTER)(Eobj_Object *object, int arg1, void *arg2, void *data);
   Eobj_Callback_INT_POINTER callback_INT_POINTER;
   int arg1;
   void *arg2;
   Eina_Bool result;

   arg1 = va_arg(arguments, int);
   arg2 = va_arg(arguments, void *);

   callback_INT_POINTER = (Eobj_Callback_INT_POINTER)callback;
   result = callback_INT_POINTER(object, arg1, arg2, data);

   *((Eina_Bool *)return_value) = result;
}
