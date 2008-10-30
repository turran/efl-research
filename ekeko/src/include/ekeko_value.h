#ifndef EKEKO_VALUE_H_
#define EKEKO_VALUE_H_

typedef struct _Edom_Value
{
	Edom_Attribute_Flag flag; /* TODO place the flags per value or per attribute ? */
	Edom_Attribute_Type type;
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
	} value; /* TODO place here the curr, prev */
} Edom_Value;

void edom_value_int_from(Edom_Value *v, int val);

#endif /*EKEKO_VALUE_H_*/
