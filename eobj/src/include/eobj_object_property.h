/** @file eobj_property.h */
#ifndef _EOBJ_PROPERTY_H_
#define _EOBJ_PROPERTY_H_

/**
 * @defgroup Eobj_Property Eobj_Property
 * @brief The property system of Eobj is a powerful system to set or to get the state of an object, or to be notified when
 * the state of an object has been changed.
 * @{
 */

/** @brief The type of a property */
typedef enum
{
   EOBJ_PROPERTY_NONE,           /**< Used when the property is uninitialized */
   EOBJ_PROPERTY_INT,            /**< The value of the property is an integer */
   EOBJ_PROPERTY_BOOL,           /**< The value of the property is a boolean (Eina_Bool) */
   EOBJ_PROPERTY_CHAR,           /**< The value of the property is a char */
   EOBJ_PROPERTY_FLOAT,          /**< The value of the property is a float */
   EOBJ_PROPERTY_DOUBLE,         /**< The value of the property is a double */
   EOBJ_PROPERTY_SHORT,          /**< The value of the property is a short */
   EOBJ_PROPERTY_LONG,           /**< The value of the property is a long */
   EOBJ_PROPERTY_OBJECT,         /**< The value of the property is an object (Eobj_Object *) */
   EOBJ_PROPERTY_POINTER,        /**< The value of the property is a pointer (void *) */
   EOBJ_PROPERTY_STRING,         /**< The value of the property is a string (char *) */
   EOBJ_PROPERTY_OTHER           /**< Used when none of the above type can be used. The property can't have a default
                                 * value, and can't be accessible with eobj_object_property_get/set().
                                 * The only interest of this kind of property is that you can still use eobj_object_notify()
                                 * when the value of the property has been modified. You then have to use the API of
                                 * the object to get the new value */
} Eobj_Property_Type;

/** @brief Describes if the property is either readable, writable or both.
 * You can also specify if the property needs to be initialized to its default value
 * at the constuction of a new object with EOBJ_PROPERTY_CONSTRUCT.
 */
typedef enum
{
   EOBJ_PROPERTY_NO_ACCESS = 1 << 0,     /**< Used when the property type is EOBJ_PROPERTY_OTHER */
   EOBJ_PROPERTY_READABLE = 1 << 1,      /**< The value of the property is readable */
   EOBJ_PROPERTY_WRITABLE = 1 << 2,      /**< The value of the property is writable */
   EOBJ_PROPERTY_READABLE_WRITABLE =     /**< The value of the property is readable and writable */
      EOBJ_PROPERTY_READABLE | EOBJ_PROPERTY_WRITABLE,
   EOBJ_PROPERTY_CONSTRUCT = 1 << 3      /**< Use this flag to force the default value of the property to be set when
                                         * the object is created */
} Eobj_Property_Flags;

/**
 * @brief An Eobj_Property is a name/value couple that can be associated to a type.
 * A property has a default value and can be readable, writable or both
 * @structinfo
 */
struct _Eobj_Property
{
   /* private: */
   char *name;
   int id;
   Eobj_Property_Type type;
   Eobj_Property_Flags flags;
   Eobj_Property_Value *default_value;
};

/**
 * @brief The value of a property. You usually do not need to manipulate it directly, use eobj_object_properties_set()
 * and eobj_object_properties_get() to set and get the value of a property of an object.
 * @structinfo
 */

struct _Eobj_Property_Value
{
   /* private: */
   union
   {
      int int_value;
      Eina_Bool bool_value;
      char char_value;
      float float_value;
      double double_value;
      short short_value;
      long long_value;
      Eobj_Object *object_value;
      void *pointer_value;
      char *string_value;
   } value;
   Eobj_Property_Type type;
};

#ifdef __cplusplus
extern "C" {
#endif

Eobj_Property       *eobj_property_new(const char *name, int property_id, Eobj_Property_Type type, Eobj_Property_Flags flags, Eobj_Property_Value *default_value);
void                eobj_property_delete(Eobj_Property *property);
Eina_Bool            eobj_property_default_value_set(Eobj_Property *property, Eobj_Property_Value *default_value);
Eobj_Property_Type   eobj_property_type_get(Eobj_Property *property);

Eobj_Property_Value *eobj_property_value_new(void);
void                eobj_property_value_delete(Eobj_Property_Value *value);
Eobj_Property_Type   eobj_property_value_type_get(Eobj_Property_Value *value);

Eobj_Property_Value *eobj_property_value_create(Eobj_Property_Type type, ...);
Eobj_Property_Value *eobj_property_value_create_valist(Eobj_Property_Type type, va_list *arg);
Eobj_Property_Value *eobj_property_value_int(int value);
Eobj_Property_Value *eobj_property_value_bool(Eina_Bool value);
Eobj_Property_Value *eobj_property_value_char(char value);
Eobj_Property_Value *eobj_property_value_float(float value);
Eobj_Property_Value *eobj_property_value_double(double value);
Eobj_Property_Value *eobj_property_value_short(short value);
Eobj_Property_Value *eobj_property_value_long(long value);
Eobj_Property_Value *eobj_property_value_object(Eobj_Object *value);
Eobj_Property_Value *eobj_property_value_pointer(void *value);
Eobj_Property_Value *eobj_property_value_string(const char *value);

void                eobj_property_value_set(Eobj_Property_Value *property_value, Eobj_Property_Type type, ...);
void                eobj_property_value_set_valist(Eobj_Property_Value *property_value, Eobj_Property_Type type, va_list *arg);
void                eobj_property_value_int_set(Eobj_Property_Value *property_value, int value);
void                eobj_property_value_bool_set(Eobj_Property_Value *property_value, Eina_Bool value);
void                eobj_property_value_char_set(Eobj_Property_Value *property_value, char value);
void                eobj_property_value_float_set(Eobj_Property_Value *property_value, float value);
void                eobj_property_value_double_set(Eobj_Property_Value *property_value, double value);
void                eobj_property_value_short_set(Eobj_Property_Value *property_value, short value);
void                eobj_property_value_long_set(Eobj_Property_Value *property_value, long value);
void                eobj_property_value_object_set(Eobj_Property_Value *property_value, Eobj_Object *value);
void                eobj_property_value_pointer_set(Eobj_Property_Value *property_value, void *value);
void                eobj_property_value_string_set(Eobj_Property_Value *property_value, const char *value);

void                eobj_property_value_get(Eobj_Property_Value *value, Eobj_Property_Type type, void *value_location);
int                 eobj_property_value_int_get(Eobj_Property_Value *value);
Eina_Bool            eobj_property_value_bool_get(Eobj_Property_Value *value);
char                eobj_property_value_char_get(Eobj_Property_Value *value);
float               eobj_property_value_float_get(Eobj_Property_Value *value);
double              eobj_property_value_double_get(Eobj_Property_Value *value);
short               eobj_property_value_short_get(Eobj_Property_Value *value);
long                eobj_property_value_long_get(Eobj_Property_Value *value);
Eobj_Object         *eobj_property_value_object_get(Eobj_Property_Value *value);
void               *eobj_property_value_pointer_get(Eobj_Property_Value *value);
const char         *eobj_property_value_string_get(Eobj_Property_Value *value);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
