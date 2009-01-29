/*
 * dummy_xml.c
 *
 *  Created on: 29-ene-2009
 *      Author: jl
 */
#include "dummy.h"
/* TODO add support for scripting */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _traverse(EXML *xml)
{

}

static void _rect_string_from(Value *v, char *r)
{
	sscanf(r, "%d %d %d %d", &v->value.rect.x, &v->value.rect.y, &v->value.rect.w, &v->value.rect.h);
}

static void _int_string_from(Value *v, char *i)
{
	sscanf(i, "0x%d", &v->value.int_value);
}

static void _attributes(void *value, void *user_data)
{
	Value val;
	Ecore_Hash_Node *n;
	Object *o;
	Property *prop;

	n = value;
	o = user_data;
	prop = object_property_get(o, n->key);
	if (!prop)
		return;
	switch (property_value_type_get(prop))
	{
		case PROPERTY_RECTANGLE:
		_rect_string_from(&val, (char *)n->value);
		break;

		case PROPERTY_INT:
		_int_string_from(&val, (char *)n->value);
		break;

		default:
		printf("Unsupported value type\n");
		return;
	}
	object_property_value_set(o, (char *)n->key, &val);
	printf("Property %s %s\n", (char *)n->key, (char *)n->value);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Object * dummy_external_load(const char *file)
{
	EXML *xml;
	EXML_Node *node;
	Object *o;

	xml = exml_new();
	exml_file_read(xml, (char *)file);
	node = exml_get(xml);
	o = type_instance_new_name_from(node->tag);
	if (!o)
		return NULL;
	/* iterate over all the attributes */
	ecore_hash_for_each_node(node->attributes, _attributes, o);
	/* traverse the childs */
	_traverse(xml);
	return o;
}
