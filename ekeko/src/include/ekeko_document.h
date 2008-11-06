#ifndef EKEKO_DOCUMENT_H_
#define EKEKO_DOCUMENT_H_

typedef void (*Ekeko_Element_New_Cb)(Ekeko_Element *e);
typedef void (*Ekeko_Element_Delete_Cb)(Ekeko_Element *e);

EAPI Ekeko_Document * ekeko_document_new(const char *ns, const char *name);
EAPI Ekeko_Element * ekeko_document_element_new(Ekeko_Document *e, const char *ns, const char *name);
EAPI Ekeko_Event * ekeko_document_event_new(Ekeko_Document *d, const char *type);
EAPI Ekeko_Document_Type * ekeko_document_register(const char *ns);
EAPI void ekeko_document_element_register(Ekeko_Document_Type *t, const char *ns,
		const char *name, Ekeko_Element_New_Cb new_cb,
		Ekeko_Element_Delete_Cb del_cb);


#endif /*EKEKO_DOCUMENT_H_*/
