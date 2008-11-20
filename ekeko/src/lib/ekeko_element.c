#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Element
{
	Ekeko_Node node;
	int changed;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Element * ekeko_element_new(Ekeko_Document *d, const char *name)
{
	Ekeko_Element *e;

	e = calloc(1, sizeof(Ekeko_Element));
	/* node initialization */
	ekeko_node_initialize((Ekeko_Node *)e);
	e->node.type = EKEKO_NODE_ELEMENT;
	e->node.owner = d;
	e->node.name = strdup(name);
	e->node.attributes = eina_hash_string_superfast_new(NULL);

	return e;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI Ekeko_Element * ekeko_element_new_ns(const char *ns)
{

}

EAPI void ekeko_element_attribute_remove(Ekeko_Element *e, const char *name)
{
	ekeko_node_attribute_remove(&e->node, name);
}


EAPI void ekeko_element_attribute_set(Ekeko_Element *e, const char *name, Ekeko_Value *v)
{
	ekeko_node_attribute_set(&e->node, name, v);
}


EAPI void ekeko_element_attribute_get(Ekeko_Element *e, const char *name, Ekeko_Value *v)
{
	ekeko_node_attribute_get(&e->node, name, v);
}

#if 0
/* LATER */
Eina_List * ekeko_element_attributes_list(Ekeko_Element *e)
{

}


Eina_Bool ekeko_element_changed(Ekeko_Element *e)
{
	if (e->changed)
		return EINA_TRUE;
	return EINA_FALSE;
}
#endif
