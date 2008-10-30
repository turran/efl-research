#ifndef EKEKO_ELEMENT_H_
#define EKEKO_ELEMENT_H_

typedef void (*Edom_Attribute_Callback)(Edom_Element *e, Edom_Value *v, void *data);
EAPI void edom_element_attribute_sync_cb_add(Edom_Element *e, const char *name,
		Edom_Attribute_Callback cb, void *data);
EAPI void edom_element_attribute_async_cb_add(Edom_Element *e, const char *name,
		Edom_Attribute_Callback cb, void *data);



EAPI Edom_Element * edom_element_new(Edom_Class *c);
EAPI Edom_Class * edom_element_class_get(Edom_Element *e);
EAPI void * edom_class_private_get(Edom_Class *c, Edom_Element *e);
EAPI const char * edom_class_name_get(Edom_Class *c);

EAPI void edom_element_delete(Edom_Element *e);
EAPI void edom_element_attribute_add(Edom_Element *e, const char *name);

EAPI void edom_element_attribute_value_set(Edom_Element *e, const char *attr,
		Edom_Value *value, Edom_Exception *ex);

EAPI void edom_element_attribute_value_get(Edom_Element *e, const char *attr,
		Edom_Value *value, Edom_Value *val, Edom_Exception *ex);

EAPI void edom_element_attribute_flags_set(Edom_Element *e, const char *attr,
		Edom_Value *value, Edom_Exception *ex);
EAPI void edom_element_attribute_get(Edom_Element *e, const char *attr,
		Edom_Value *v, Edom_Exception *ex);
Eina_Bool edom_element_child_append(Edom_Element *e, Edom_Element *child, Edom_Exception *ex);
EAPI const char * edom_element_namespace_get(Edom_Element *e);
EAPI const char * edom_element_name_get(Edom_Element *e);

#endif /*EKEKO_ELEMENT_H_*/
