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
};

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

}
EAPI Ekeko_Element * ekeko_document_element_new(Ekeko_Document *e, const char *ns, const char *name)
{
	
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
		t->ns = strdup(ns);
		eina_hash_add(_dtypes, t, ns);
	}
	return t;
}

EAPI void ekeko_document_element_register(Ekeko_Document_Type *t, const char *ns,
		const char *name, Ekeko_Element_New_Cb new_cb,
		Ekeko_Element_Delete_Cb del_cb)
{
	
}
