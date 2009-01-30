#include "Ekeko.h"
#include "ekeko_private.h"
/**
 * Misses DOCTYPE information
 * handle the element and event delete functions
 * feed UI events
 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Document_Type
{
	const char *ns;
	Eina_Hash *elements;
#ifdef EKEKO_EVENT
	Eina_Hash *events;
#endif
};

struct _Ekeko_Element_Type
{
	Ekeko_Element_New_Cb new_cb;
	Ekeko_Element_Delete_Cb del_cb;
	Eina_Hash *attributes;
};

typedef struct _Ekeko_Event_Type
{
	Ekeko_Event_New_Cb new_cb;
	Ekeko_Event_Delete_Cb del_cb;
} Ekeko_Event_Type;

struct _Ekeko_Document
{
	Ekeko_Node node;
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
	/* node initialization */
	ekeko_node_initialize(&d->node);
	d->node.type = EKEKO_NODE_DOCUMENT;
	d->node.name = strdup(name);
	d->node.changed = 0;
	
	return d;
}

EAPI Ekeko_Element * ekeko_document_element_new(Ekeko_Document *d, const char *ns, const char *name)
{
	Ekeko_Element_Type *t;
	Ekeko_Element *e;
	Eina_Hash_Tuple *data;
	Eina_Iterator *it;

	/* create the element */
	t = eina_hash_find(d->type->elements, name);
	if (!t) return NULL;
	
	e = ekeko_element_new(d, name);
	/* create the default attributes */
	it = eina_hash_iterator_tuple_new(t->attributes);
	while (eina_iterator_next(it, (void **)&data))
	{
		ekeko_node_attribute_set((Ekeko_Node *)e, data->key, data->data);
	}
	t->new_cb(e);
	return e;
}

EAPI Ekeko_Event * ekeko_document_event_new(Ekeko_Document *d, const char *name)
{
	Ekeko_Event_Type *t = NULL;

#ifdef EKEKO_EVENT
	t = eina_hash_find(d->type->events, name);
#endif
	if (!t) return NULL;
		
	return t->new_cb();
}
/* Document Type interface */
/* To register a new document type and elements */
EAPI Ekeko_Document_Type * ekeko_document_type_register(const char *ns)
{
	Ekeko_Document_Type *t;

	t = eina_hash_find(_dtypes, ns);
	if (!t)
	{
		t = malloc(sizeof(Ekeko_Document_Type));
		t->elements = eina_hash_string_superfast_new(NULL);
		t->ns = strdup(ns);
		eina_hash_add(_dtypes, ns, t);
		/* setup the common events */
#ifdef EKEKO_EVENT
		t->events = eina_hash_string_superfast_new(NULL);
		ekeko_event_setup(t);
#endif
	}
	return t;
}

/**
 * FIXME what to do with the namespace?
 * TODO add a callback for adding a child
 */
EAPI Ekeko_Element_Type * ekeko_document_type_element_register(Ekeko_Document_Type *t, const char *ns,
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
		et->attributes = eina_hash_string_superfast_new(NULL);
		eina_hash_add(t->elements, name, et);
	}
	return et;
}
/**
 * add a way to add attributes for deafult values and automatic insert in case
 * an attribute has been deleted
 */
EAPI void ekeko_document_type_element_attribute_register(Ekeko_Element_Type *et,
		const char *name, Ekeko_Value_Type type, void *data)
{
	Ekeko_Value *n;
	
	n = ekeko_value_new(type, data);
	eina_hash_add(et->attributes, name, n);
}

EAPI void ekeko_document_type_event_register(Ekeko_Document_Type *t, const char *name,
		Ekeko_Event_New_Cb new_cb, Ekeko_Event_Delete_Cb del_cb)
{
	Ekeko_Event_Type *et;

#ifdef EKEKO_EVENT
	et = eina_hash_find(t->events, name);
	if (!et)
	{
		et = malloc(sizeof(Ekeko_Event_Type));
		et->new_cb = new_cb;
		et->del_cb = del_cb;
		eina_hash_add(t->events, name, et);
	}
#endif	
}
