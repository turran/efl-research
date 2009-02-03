/*
 * etk_value.h
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */

#ifndef ETK_VALUE_H_
#define ETK_VALUE_H_

/**
 * @brief Possible types of a property.
 */
#define PROPERTY_INT 1 /**< The value of the property is an integer */          //!< PROPERTY_INT
#define PROPERTY_BOOL 2 /**< The value of the property is a boolean (Bool) */    //!< PROPERTY_BOOL
#define PROPERTY_CHAR 3 /**< The value of the property is a char */              //!< PROPERTY_CHAR
#define PROPERTY_FLOAT 4 /**< The value of the property is a float */             //!< PROPERTY_FLOAT
#define PROPERTY_DOUBLE 5 /**< The value of the property is a double */            //!< PROPERTY_DOUBLE
#define PROPERTY_SHORT 6 /**< The value of the property is a short */             //!< PROPERTY_SHORT
#define PROPERTY_LONG 7 /**< The value of the property is a long */              //!< PROPERTY_LONG
#define PROPERTY_RECTANGLE 8 /**< The value of the property is a rectangle */         //!< PROPERTY_RECTANGLE
#define PROPERTY_POINTER 9 /**< The value of the property is a pointer (void *) */  //!< PROPERTY_POINTER
#define PROPERTY_STRING 10 /**< The value of the property is a string (char *) */ //!< PROPERTY_STRING

typedef int Value_Type;
/**
 * @brief
 */
typedef struct _Value
{
	union
	{
		int int_value;
		Eina_Bool bool_value;
		char char_value;
		float float_value;
		double double_value;
		short short_value;
		long long_value;
		void *pointer_value;
		char *string_value;
		Eina_Rectangle rect;
	} value;
} Value;

typedef void (*Ekeko_Value_Set)(Value *v, void *val);
typedef void (*Ekeko_Value_Pointer_Set)(Value *v, void *ptr);
typedef Eina_Bool (*Ekeko_Value_Compare)(void *a, void *b);

void ekeko_value_set(Value *v, Value_Type vtype, void *val);
int ekeko_value_register(const char *name, Ekeko_Value_Set set,
		Ekeko_Value_Pointer_Set pset, Ekeko_Value_Compare cmp);

static inline void value_int_from(Value *v, int i)
{
	v->value.int_value = i;
}

static inline void value_str_from(Value *v, char *str)
{
	v->value.string_value = str;
}

static inline void value_rectangle_from(Value *v, Eina_Rectangle *rect)
{
	v->value.rect = *rect;
}

static inline void value_rectangle_coords_from(Value *v, int x, int y, int w, int h)
{
	v->value.rect.x = x;
	v->value.rect.y = y;
	v->value.rect.w = w;
	v->value.rect.h = h;
}

static inline void value_bool_from(Value *v, Eina_Bool b)
{
	v->value.bool_value = b;
}

#endif /* ETK_VALUE_H_ */
