#ifndef EKEKO_VALUE_H_
#define EKEKO_VALUE_H_


typedef enum _Ekeko_Value_Type
{
	EKEKO_ATTRIBUTE_NONE, /**< Used when the property is uninitialized */
	EKEKO_ATTRIBUTE_INT, /**< The value of the property is an integer */
	EKEKO_ATTRIBUTE_BOOL, /**< The value of the property is a boolean (Eina_Bool) */
	EKEKO_ATTRIBUTE_CHAR, /**< The value of the property is a char */
	EKEKO_ATTRIBUTE_FLOAT, /**< The value of the property is a float */
	EKEKO_ATTRIBUTE_DOUBLE, /**< The value of the property is a double */
	EKEKO_ATTRIBUTE_SHORT, /**< The value of the property is a short */
	EKEKO_ATTRIBUTE_LONG, /**< The value of the property is a long */
	EKEKO_ATTRIBUTE_POINTER, /**< The value of the property is a pointer (void *) */
	EKEKO_ATTRIBUTE_STRING, /**< The value of the property is a string (char *) */	
	EKEKO_ATTRIBUTE_RECTANGLE,
	// EKEKO_ATTRIBUTE_ELEMENT, How to handle the compare? per pointer + changed flag?
	EKEKO_ATTRIBUTES,
} Ekeko_Value_Type;

typedef struct _Ekeko_Value
{
	Ekeko_Value_Type type;
	union {
		int i;
		Eina_Bool b;
		char c;
		float f;
		double d;
		short s;
		long l;
		void *p;
		char *st;
	} v;
} Ekeko_Value;

typedef Eina_Bool (*Ekeko_Value_Compare)(const Ekeko_Value *a, const Ekeko_Value *b);

void ekeko_value_int_from(Ekeko_Value *v, int i);
void ekeko_value_rectangle_from(Ekeko_Value *v, Eina_Rectangle *r);
void ekeko_value_string_from(Ekeko_Value *v, const char *string);

#endif /*EKEKO_VALUE_H_*/
