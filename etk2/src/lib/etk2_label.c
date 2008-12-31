#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "etk2_private.h"
#include "etk2_types.h"
#include "etk2_label.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(l) ((l)->private)
#define TYPE_NAME "Label"

struct _Label_Private
{
	char *text;
};

static void label_ctor(Type *t, void *instance)
{
	Label *label = (Label*) instance;

	label->private = type_instance_private_get(t, label_type_get(), instance);
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
	type_construct(label_type_get(), label);
	printf("3.1 label = %p, private = %p\n", label, label->private);
	printf("3.2 label = %p, private = %p\n", label, label->private);

	return label;
}

void label_text_set(Label *label, char *text)
{
	Label_Private *private;

	RETURN_IF(label == NULL);

	private = PRIVATE(label);
	printf("4 label = %p, private = %p\n", label, private);
	private->text = strdup(text);
}

char *label_text_get(Label *label)
{
	Label_Private *private;

	RETURN_NULL_IF(label == NULL);

	private = PRIVATE(label);
	return private->text;
}
