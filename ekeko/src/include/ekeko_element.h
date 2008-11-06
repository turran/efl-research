#ifndef EKEKO_ELEMENT_H_
#define EKEKO_ELEMENT_H_

typedef void (*Ekeko_Element_Update)(Ekeko_Element *, const Ekeko_Value *, const Ekeko_Value *, void *);

typedef void (*Ekeko_Attribute_Callback)(Ekeko_Element *e, Ekeko_Value *v, void *data);
EAPI void ekeko_element_attribute_sync_cb_add(Ekeko_Element *e, const char *name,
		Ekeko_Attribute_Callback cb, void *data);
EAPI void ekeko_element_attribute_async_cb_add(Ekeko_Element *e, const char *name,
		Ekeko_Attribute_Callback cb, void *data);

Ekeko_Element * ekeko_element_new(void);
void ekeko_element_process(Ekeko_Element *e);
void ekeko_element_attribute_remove(Ekeko_Element *e, const char *name);
void ekeko_element_attribute_add(Ekeko_Element *e, const char *name, Ekeko_Value_Type type,
		Ekeko_Value *def, Ekeko_Element_Update cb, void *data);
Eina_Bool ekeko_element_attribute_set(Ekeko_Element *e, const char *name, Ekeko_Value *v);
Eina_Bool ekeko_element_attribute_get(Ekeko_Element *e, const char *name, Ekeko_Value *v);


#endif /*EKEKO_ELEMENT_H_*/
