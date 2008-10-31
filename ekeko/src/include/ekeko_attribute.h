#ifndef EKEKO_ATTRIBUTE_H_
#define EKEKO_ATTRIBUTE_H_

typedef enum _Ekeko_Attribute_Flags
{
	EKEKO_ATTRIBUTE_ABSOLUTE  = (1 << 0),
	EKEKO_ATTRIBUTE_RELATIVE  = (1 << 1),
	EKEKO_ATTRIBUTE_INHERITED = (1 << 2),
} Ekeko_Attribute_Flag;

#endif /*EKEKO_ATTRIBUTE_H_*/
