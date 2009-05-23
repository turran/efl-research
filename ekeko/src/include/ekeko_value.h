/*
 * etk_value.h
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */

#ifndef EKEKO_VALUE_H_
#define EKEKO_VALUE_H_

/**
 * @brief Possible types of a property.
 */
#define PROPERTY_UNDEFINED 0
#define PROPERTY_INT 1 /**< The value of the property is an integer */          //!< PROPERTY_INT
#define PROPERTY_BOOL 2 /**< The value of the property is a boolean (Bool) */    //!< PROPERTY_BOOL
#define PROPERTY_CHAR 3 /**< The value of the property is a char */              //!< PROPERTY_CHAR
#define PROPERTY_FLOAT 4 /**< The value of the property is a float */             //!< PROPERTY_FLOAT
#define PROPERTY_DOUBLE 5 /**< The value of the property is a double */            //!< PROPERTY_DOUBLE
#define PROPERTY_SHORT 6 /**< The value of the property is a short */             //!< PROPERTY_SHORT
#define PROPERTY_LONG 7 /**< The value of the property is a long */              //!< PROPERTY_LONG
#define PROPERTY_RECTANGLE 8 /**< The value of the property is a rectangle */         //!< PROPERTY_RECTANGLE
#define PROPERTY_POINTER 9 /**< The value of the property is a pointer (void *) */  //!< PROPERTY_POINTER
#define PROPERTY_VALUE 10 /**< The value of the property is value */  //!< PROPERTY_VALUE
#define PROPERTY_STRING 11 /**< The value of the property is a string (char *) */ //!< PROPERTY_STRING
#define PROPERTY_OBJECT 12 /**< The value of the property is an Object */ //!< PROPERTY_OBJECT
#define PROPERTY_LAST PROPERTY_OBJECT

typedef int Ekeko_Value_Type;
/**
 * @brief
 */
typedef struct _Ekeko_Value
{
	Ekeko_Value_Type type;
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
		Ekeko_Object *object;
	} value;
} Ekeko_Value;

typedef void * (*Ekeko_Value_Create)(void);
typedef void (*Ekeko_Value_Free)(void *v);
typedef Eina_Bool (*Ekeko_Value_Compare)(void *a, void *b);
typedef void (*Ekeko_Value_Pointer_From)(Ekeko_Value *v, void *val);
typedef void (*Ekeko_Value_Pointer_To)(Ekeko_Value *v, void *ptr);

#define EKEKO_VALUE_CREATE(f) ((Ekeko_Value_Create)(f))
#define EKEKO_VALUE_FREE(f) ((Ekeko_Value_Free)(f))
#define EKEKO_VALUE_CMP(f) ((Ekeko_Value_Compare)(f))
#define EKEKO_VALUE_POINTER_FROM(f) ((Ekeko_Value_Pointer_From)(f))
#define EKEKO_VALUE_POINTER_TO(f) ((Ekeko_Value_Pointer_To)(f))

void ekeko_value_pointer_from(Ekeko_Value *v, Ekeko_Value_Type vtype, void *val);
void ekeko_value_free(Ekeko_Value *v, Ekeko_Value_Type vtype);
int ekeko_value_register(const char *name, Ekeko_Value_Create create,
		Ekeko_Value_Free free, Ekeko_Value_Compare cmp,
		Ekeko_Value_Pointer_From pointer_from,
		Ekeko_Value_Pointer_To pointer_to);

static inline void ekeko_value_int_from(Ekeko_Value *v, int i)
{
	v->type = PROPERTY_INT;
	v->value.int_value = i;
}

static inline void ekeko_value_float_from(Ekeko_Value *v, float f)
{
	v->type = PROPERTY_FLOAT;
	v->value.float_value = f;
}

static inline void ekeko_value_str_from(Ekeko_Value *v, char *str)
{
	v->type = PROPERTY_STRING;
	v->value.string_value = str;
}

static inline void ekeko_value_rectangle_from(Ekeko_Value *v, Eina_Rectangle *rect)
{
	v->type = PROPERTY_RECTANGLE;
	v->value.rect = *rect;
}

static inline void ekeko_value_rectangle_coords_from(Ekeko_Value *v, int x, int y, int w, int h)
{
	v->type = PROPERTY_RECTANGLE;
	v->value.rect.x = x;
	v->value.rect.y = y;
	v->value.rect.w = w;
	v->value.rect.h = h;
}

static inline void ekeko_value_bool_from(Ekeko_Value *v, Eina_Bool b)
{
	v->type = PROPERTY_BOOL;
	v->value.bool_value = b;
}


static inline void ekeko_value_object_from(Ekeko_Value *v, Ekeko_Object *o)
{
	v->type = PROPERTY_OBJECT;
	v->value.object = o;
}

#endif /* EKEKO_VALUE_H_ */
