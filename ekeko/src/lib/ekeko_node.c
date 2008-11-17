#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
void _attribute_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	int changed = EINA_FALSE;

	switch (v->type)
	{
	/* First basic types */
	case EKEKO_ATTRIBUTE_INT:
		if (v->v.i != n->attr.prev.v.i)
		{
			changed = EINA_TRUE;
			n->value.v.i = v->v.i;
		}
		break;
	/* TODO User provided ones */
	default:
		break;
	}
	if (changed == EINA_TRUE)
	{
		/* TODO call the pre-mutation event */
	}
	n->changed = changed;
	return;
}
void _attribute_process(Ekeko_Node *n)
{
	if (!n->changed)
		return;

	printf("Attribute changed %d\n", n->changed);
	/* TODO call the mutation event */
	/* update prev */
	n->attr.prev = n->value;
	n->changed = EINA_FALSE;
}
void _element_process(Ekeko_Node *n)
{
	Eina_Iterator *it;
	Ekeko_Node *in;

	printf("Element changed %d\n", n->changed);
	if (!n->changed)
		return;
	/* all attributes */
	it = eina_hash_iterator_data_new(n->attributes);
	while (eina_iterator_next(it, (void **)&in))
	{
		if (ekeko_node_changed(in))
		{
			_attribute_process(in);
			n->changed--;
		}
	}
	/* all childs */
	it = eina_list_iterator_new(n->childs);
	while (eina_iterator_next(it, (void **)&in))
	{
		ekeko_node_process(in);
	}
	/* post condition */
	assert(!n->changed);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_node_initialize(Ekeko_Node *n)
{
	n->user = eina_hash_string_superfast_new(NULL);
}

Eina_Bool ekeko_node_changed(Ekeko_Node *n)
{
	if (n->changed > 0)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * 
 */
EAPI void ekeko_node_value_get(Ekeko_Node *n, Ekeko_Value *v)
{
	*v = n->value;
}
/**
 * 
 * 
 */
EAPI void ekeko_node_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	switch (n->type)
	{
	case EKEKO_NODE_ATTRIBUTE:
		_attribute_value_set(n, v);
		break;
	}
}
/**
 * 
 */
EAPI Ekeko_Node_Type ekeko_node_type(Ekeko_Node *n)
{
	return n->type;
}
EAPI Ekeko_Node * ekeko_node_parent(Ekeko_Node *n)
{
	
}
/* LEVEL 2 */
EAPI Ekeko_Document * ekeko_node_document_owner(Ekeko_Node *n)
{
	
}
/* LEVEL 3 */
EAPI void ekeko_node_user_set(Ekeko_Node *n, const char *key, void *data)
{
	eina_hash_add(n->user, key, data);
}
EAPI void * ekeko_node_user_get(Ekeko_Node *n, const char *key)
{
	eina_hash_find(n->user, key);
}

/* Own */
EAPI void ekeko_node_process(Ekeko_Node *n)
{
	/* attributes dont have any parent, childs or siblings */
	switch (n->type)
	{
	case EKEKO_NODE_ELEMENT:
		_element_process(n);
		break;
	}
}
