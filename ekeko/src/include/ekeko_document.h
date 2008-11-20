#ifndef EKEKO_DOCUMENT_H_
#define EKEKO_DOCUMENT_H_

typedef void (*Ekeko_Element_New_Cb)(Ekeko_Element *e);
typedef void (*Ekeko_Element_Delete_Cb)(Ekeko_Element *e);
typedef Ekeko_Event * (*Ekeko_Event_New_Cb)(void);
typedef void (*Ekeko_Event_Delete_Cb)(Ekeko_Event *e);

EAPI Ekeko_Document * ekeko_document_new(const char *ns, const char *name);
EAPI Ekeko_Element * ekeko_document_element_new(Ekeko_Document *e, const char *ns, const char *name);
EAPI Ekeko_Event * ekeko_document_event_new(Ekeko_Document *d, const char *type);

EAPI Ekeko_Document_Type * ekeko_document_type_register(const char *ns);
EAPI Ekeko_Element_Type * ekeko_document_type_element_register(Ekeko_Document_Type *t, const char *ns,
		const char *name, Ekeko_Element_New_Cb new_cb,
		Ekeko_Element_Delete_Cb del_cb);
EAPI void ekeko_document_type_element_attribute_register(Ekeko_Element_Type *et, const char *name, Ekeko_Value *def);
EAPI void ekeko_document_type_event_register(Ekeko_Document_Type *t, const char *name,
		Ekeko_Event_New_Cb new_cb, Ekeko_Event_Delete_Cb del_cb);


#endif /*EKEKO_DOCUMENT_H_*/
