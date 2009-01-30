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

static Ekeko_Attribute * _attribute_get(Ekeko_Element *e, const char *name)
{
	Ekeko_Attribute *a;

	a = ekeko_element_attribute_node_get(e, name);
	if (!a)
	{
		a = ekeko_attribute_new(e, name);
		// ekeko_node_attribute_add((Ekeko_Node *)e, name);
		eina_hash_add(((Ekeko_Node *)e)->attributes, name, a);
	}
	return a;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Element * ekeko_element_new(Ekeko_Document *d, const char *name)
{
	Ekeko_Element *e;

	e = calloc(1, sizeof(Ekeko_Element));
	ekeko_node_element_new((Ekeko_Node *)e, name, d);

	return e;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI Ekeko_Element * ekeko_element_new_ns(const char *ns)
{

}

EAPI Ekeko_Attribute * ekeko_element_attribute_node_get(Ekeko_Element *e, const char *name)
{
	Ekeko_Attribute *a;
	
	//ekeko_node_attribute_get((Ekeko_Node *)e, name);
	a = eina_hash_find(((Ekeko_Node *)e)->attributes, name);
	return a;
}

EAPI void ekeko_element_attribute_remove(Ekeko_Element *e, const char *name)
{
	ekeko_node_attribute_remove(&e->node, name);
}


EAPI void ekeko_element_attribute_set(Ekeko_Element *e, const char *name, const char *val)
{
	Ekeko_Attribute *a;

	a = _attribute_get(e, name);
	ekeko_attribute_value_set(a, val);
}

EAPI void ekeko_element_attribute_int_set(Ekeko_Element *e, const char *name, int val)
{
	Ekeko_Attribute *a;
	
	a = _attribute_get(e, name);
	ekeko_attribute_value_int_set(a, val);
}

EAPI void ekeko_element_attribute_rectangle_set(Ekeko_Element *e, const char *name,
		Eina_Rectangle *val)
{
	Ekeko_Attribute *a;
	
	a = _attribute_get(e, name);
	ekeko_attribute_value_rectangle_set(a, val);
}

EAPI const Ekeko_Value * ekeko_element_attribute_get(Ekeko_Element *e, const char *name)
{
	Ekeko_Attribute *a;

	//ekeko_node_attribute_get((Ekeko_Node *)e, name);
	a = eina_hash_find(((Ekeko_Node *)e)->attributes, name);
	return ekeko_attribute_value_get(a);
}

#if 0
/* LATER */
Eina_List * ekeko_element_attributes_list(Ekeko_Element *e)
{

}
#endif
