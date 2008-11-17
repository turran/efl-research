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
Ekeko_Attribute * ekeko_attribute_new(const char *name, Ekeko_Value *def)
{
	Ekeko_Attribute *a;

	a = calloc(1, sizeof(Ekeko_Attribute));
	//a->node.attr.changed = EINA_FALSE;
	/*
	if (def)
		a->curr = a->prev = *def;*/

	return a;
}
Eina_Bool ekeko_attribute_changed(Ekeko_Attribute *a)
{
	//return a->node.attr.changed;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void ekeko_attribute_value_set(Ekeko_Attribute *a, Ekeko_Value *v)
{
	ekeko_node_value_set(&a->node, v);
}

EAPI void ekeko_attribute_value_get(Ekeko_Attribute *a, Ekeko_Value *v)
{
	ekeko_node_value_get(&a->node, v);
}
