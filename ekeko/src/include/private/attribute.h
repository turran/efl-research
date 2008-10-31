#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

typedef struct _Ekeko_Attribute Ekeko_Attribute;

Ekeko_Attribute * ekeko_attribute_new(Ekeko_Value_Type type, Ekeko_Value *def,
		Ekeko_Element_Update cb, void *data);
Eina_Bool ekeko_attribute_changed(Ekeko_Attribute *a);
Eina_Bool ekeko_attribute_update(Ekeko_Attribute *a, Ekeko_Element *e);
Eina_Bool ekeko_attribute_value_set(Ekeko_Attribute *e, Ekeko_Value *v);
void ekeko_attribute_value_get(Ekeko_Attribute *a, Ekeko_Value *v);

#endif /*ATTRIBUTE_H_*/
