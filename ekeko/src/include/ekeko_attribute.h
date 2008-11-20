#ifndef EKEKO_ATTRIBUTE_H_
#define EKEKO_ATTRIBUTE_H_

typedef enum _Ekeko_Attribute_Flags
{
	EKEKO_ATTRIBUTE_ABSOLUTE  = (1 << 0),
	EKEKO_ATTRIBUTE_RELATIVE  = (1 << 1),
	EKEKO_ATTRIBUTE_INHERITED = (1 << 2),
} Ekeko_Attribute_Flag;

EAPI Ekeko_Element * ekeko_attribute_element_get(Ekeko_Attribute *a);


#endif /*EKEKO_ATTRIBUTE_H_*/
