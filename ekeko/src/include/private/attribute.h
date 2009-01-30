#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

Ekeko_Attribute * ekeko_attribute_new(Ekeko_Element *e, const char *name);
Eina_Bool ekeko_attribute_changed(Ekeko_Attribute *a);
Eina_Bool ekeko_attribute_update(Ekeko_Attribute *a, Ekeko_Element *e);
void ekeko_attribute_value_set(Ekeko_Attribute *e, const char *v);

#endif /*ATTRIBUTE_H_*/
