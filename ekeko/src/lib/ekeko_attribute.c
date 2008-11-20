#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Attribute
{
	Ekeko_Node node;
	Ekeko_Element *owner;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Attribute * ekeko_attribute_new(Ekeko_Element *e, const char *name, Ekeko_Value *def)
{
	Ekeko_Attribute *a;

	a = calloc(1, sizeof(Ekeko_Attribute));
	a->owner = e;
	/* node initialization */
	ekeko_node_initialize((Ekeko_Node *)a);
	a->node.type = EKEKO_NODE_ATTRIBUTE;
	a->node.name = strdup(name);
	a->node.owner = ((Ekeko_Node *)e)->owner;
	a->node.changed = EINA_TRUE;

	if (def)
	{
		a->node.value = a->node.attr.prev = *def;
	}
		
	return a;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Element * ekeko_attribute_element_get(Ekeko_Attribute *a)
{
	return a->owner;
}

EAPI void ekeko_attribute_value_set(Ekeko_Attribute *a, Ekeko_Value *v)
{
	ekeko_node_value_set(&a->node, v);
}

EAPI void ekeko_attribute_value_get(Ekeko_Attribute *a, Ekeko_Value *v)
{
	ekeko_node_value_get(&a->node, v);
}
