#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

Edom_Attribute * edom_attribute_new(Edom_Attribute_Type type, Edom_Value *def,
		Edom_Attribute_Update cb, void *data);
Eina_Bool edom_attribute_changed(Edom_Attribute *a);
Eina_Bool edom_attribute_update(Edom_Attribute *a, Edom_Element *e);
Eina_Bool edom_attribute_value_set(Edom_Attribute *e, Edom_Value *v);
void edom_attribute_value_get(Edom_Attribute *a, Edom_Value *v);

#endif /*ATTRIBUTE_H_*/
