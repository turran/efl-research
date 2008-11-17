#include "Ekeko.h"
#include "ekeko_private.h"
/**
 * Misses DOCTYPE information
 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Document_Type
{
	const char *ns;
	Eina_Hash *elements;
};

typedef struct _Ekeko_Element_Type
{
	Ekeko_Element_New_Cb new_cb;
	Ekeko_Element_Delete_Cb del_cb;
} Ekeko_Element_Type;

struct _Ekeko_Document
{
	Ekeko_Document_Type *type;
};
Eina_Hash *_dtypes;

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_document_init(void)
{
	_dtypes = eina_hash_string_superfast_new(NULL);
}
void ekeko_document_shutdown(void)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Document * ekeko_document_new(const char *ns, const char *name)
{
	Ekeko_Document *d;
	Ekeko_Document_Type *dt;
	
	dt = eina_hash_find(_dtypes, ns);
	if (!dt) return NULL;
	
	d = malloc(sizeof(Ekeko_Document));
	d->type = dt;
	return d;
}

EAPI Ekeko_Element * ekeko_document_element_new(Ekeko_Document *d, const char *ns, const char *name)
{
	Ekeko_Element_Type *t;
	Ekeko_Element *e;
	
	t = eina_hash_find(d->type->elements, name);
	if (!t) return NULL;
	
	e = ekeko_element_new();
	t->new_cb(e);
	return e;
}

EAPI Ekeko_Event * ekeko_document_event_new(Ekeko_Document *d, const char *type)
{

}
/* To register a new document type and elements */
EAPI Ekeko_Document_Type * ekeko_document_register(const char *ns)
{
	Ekeko_Document_Type *t;

	t = eina_hash_find(_dtypes, ns);
	if (!t)
	{
		t = malloc(sizeof(Ekeko_Document_Type));
		t->elements = eina_hash_string_superfast_new(NULL);
		t->ns = strdup(ns);
		eina_hash_add(_dtypes, ns, t);
	}
	return t;
}

/**
 * FIXME what to do with the namespace?
 */
EAPI void ekeko_document_element_register(Ekeko_Document_Type *t, const char *ns,
		const char *name, Ekeko_Element_New_Cb new_cb,
		Ekeko_Element_Delete_Cb del_cb)
{
	Ekeko_Element_Type *et;
	
	et = eina_hash_find(t->elements, name);
	if (!et)
	{
		et = malloc(sizeof(Ekeko_Element_Type));
		et->new_cb = new_cb;
		et->del_cb = del_cb;
		eina_hash_add(t->elements, name, et);
	}
}
