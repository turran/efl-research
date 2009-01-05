#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(l) ((l)->private)
#define TYPE_NAME "Label"

struct _Label_Private
{
	char *text;
};

static void label_ctor(void *instance)
{
	Label *label = (Label*) instance;

	label->private = type_instance_private_get(label_type_get(), instance);
	label->private->text = NULL;
	printf("[label] ctor %p %p\n", label, label->private);
}

static void label_dtor(void *label)
{
}

static void label_property_value_set(Object *object, char *prop_name, Type_Property_Value *value)
{
}

static Type_Property_Value *label_property_value_get(Object *object, char *prop_name)
{
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *label_type_get(void)
{
	static Type *label_type = NULL;

	if (!label_type)
	{
		label_type = type_new(TYPE_NAME, sizeof(Label), sizeof(Label_Private), widget_type_get(), label_ctor, label_dtor, label_property_value_set, label_property_value_get);
		type_property_new(label_type, "text", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Label_Private, text), NULL);
	}

	return label_type;
}

Label *label_new(void)
{
	Label *label;

	label = type_instance_new(label_type_get());

	return label;
}

void label_text_set(Label *label, char *text)
{
	Type_Property_Value v;

	RETURN_IF(label == NULL);
	/* FIXME how to inform the property system that this property has changed ?
	 * for now we do this way but is slow because we need to get the property
	 * from the hash even knowing what offset we are working with
	 */
	value_str_from(&v, text);
	object_property_value_set((Object *)label, "text", &v);
}

char *label_text_get(Label *label)
{
	Label_Private *private;

	RETURN_NULL_IF(label == NULL);

	private = PRIVATE(label);
	return private->text;
}
