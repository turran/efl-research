#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Attribute
{
	Ekeko_Node node;
	Ekeko_Element *owner;
	Eina_Bool specified; /* if the value has been actually set */
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Attribute * ekeko_attribute_new(Ekeko_Element *e, const char *name)
{
	Ekeko_Attribute *a;

	a = calloc(1, sizeof(Ekeko_Attribute));
	a->owner = e;
	/* node initialization */
	ekeko_node_attribute_new((Ekeko_Node *)a, name, ((Ekeko_Node *)e)->owner);

	return a;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Element * ekeko_attribute_element_get(Ekeko_Attribute *a)
{
	return a->owner;
}
EAPI Ekeko_Value_Type ekeko_attribute_type_get(Ekeko_Attribute *a)
{
	
}
EAPI void ekeko_attribute_value_set(Ekeko_Attribute *a, const char *val)
{
	ekeko_node_value_set(&a->node, val);
	a->specified = EINA_TRUE;
}

EAPI void ekeko_attribute_value_int_set(Ekeko_Attribute *a, int val)
{
	ekeko_node_value_int_set(&a->node, val);
	a->specified = EINA_TRUE;
}

EAPI const Ekeko_Value * ekeko_attribute_value_get(Ekeko_Attribute *a)
{
	return ekeko_node_value_get(&a->node);
}

EAPI Eina_Bool ekeko_attribute_specified(Ekeko_Attribute *a)
{
	
}
