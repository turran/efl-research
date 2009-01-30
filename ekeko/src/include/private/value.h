#ifndef VALUE_H_
#define VALUE_H_

Ekeko_Value * ekeko_value_new(Ekeko_Value_Type type, void *d);
Ekeko_Value_Class * ekeko_value_class_get(Ekeko_Value_Type t);

#endif /*VALUE_H_*/
