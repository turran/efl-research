#ifndef EKEKO_VALUE_H_
#define EKEKO_VALUE_H_

typedef int Ekeko_Value_Type;

#define EKEKO_ATTRIBUTE_NONE 0
#define EKEKO_ATTRIBUTE_INT 1
#define EKEKO_ATTRIBUTE_BOOL 2
#define EKEKO_ATTRIBUTE_CHAR 3
#define EKEKO_ATTRIBUTE_FLOAT 4
#define EKEKO_ATTRIBUTE_DOUBLE 5
#define EKEKO_ATTRIBUTE_SHORT 6
#define EKEKO_ATTRIBUTE_LONG 7
#define EKEKO_ATTRIBUTE_POINTER 8
#define EKEKO_ATTRIBUTE_STRING 9
#define EKEKO_ATTRIBUTE_RECTANGLE 10
#define EKEKO_ATTRIBUTE_LAST 10
/* TODO add an inherited type */

#if 0
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
#endif

typedef struct _Ekeko_Value Ekeko_Value;
/**
 * Used to register a new value
 */
typedef struct _Ekeko_Value_Class
{
	Eina_Bool (*compare)(Ekeko_Value *v1, Ekeko_Value *v2);
	void (*string_from)(Ekeko_Value *v, const char *);
	void (*string_to)(Ekeko_Value *v, const char **);
} Ekeko_Value_Class;

typedef Eina_Bool (*Ekeko_Value_Compare)(const Ekeko_Value *a, const Ekeko_Value *b);

void ekeko_value_int_from(Ekeko_Value *v, int i);
void ekeko_value_rectangle_from(Ekeko_Value *v, Eina_Rectangle *r);
void ekeko_value_string_from(Ekeko_Value *v, const char *string);
void ekeko_value_external_from(Ekeko_Value *v, Ekeko_Value_Type t, void *ext);

EAPI int ekeko_value_int_get(Ekeko_Value *v);
EAPI void * ekeko_value_external_get(Ekeko_Value *v);
EAPI void ekeko_value_rectangle_get(Ekeko_Value *v, Eina_Rectangle *r);


#endif /*EKEKO_VALUE_H_*/
