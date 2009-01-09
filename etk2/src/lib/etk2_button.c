#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(b) ((b)->private)
#define TYPE_NAME "Button"

struct _Button_Private
{
	int foo;
	char *label;
};
static void button_ctor(void *button)
{
	 Button *btn;

	 btn = (Button*) button;
	 btn->private = type_instance_private_get(button_type_get(), btn);
	 printf("[btn] ctor %p %p\n", btn, btn->private);
}

static void button_dtor(void *button)
{
	printf("[btn] dtor %p\n", button);
}

static void button_property_value_set(Object *object, char *prop_name, Value *value)
{
	printf("[btn] value_set: %s\n", prop_name);
}

static Value *button_property_value_get(Object *object, char *prop_name)
{
	printf("[btn] value_get: %s\n", prop_name);
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *button_type_get(void)
{
	static Type *button_type = NULL;

	if (!button_type)
	{
		button_type = type_new(TYPE_NAME, sizeof(Button), sizeof(Button_Private), widget_type_get(), button_ctor, button_dtor, button_property_value_set, button_property_value_get);
		type_property_new(button_type, "label", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Button_Private, label), NULL);
	}

	return button_type;
}

Button *button_new(void)
{
	Button *button;

	button = type_instance_new(button_type_get());

	return button;
}

void button_label_set(Button *button, const char *label)
{
	Button_Private *private;

	private = PRIVATE(button);

	if (label)
		private->label = strdup(label);
}

const char *button_label_get(Button *button)
{
	Button_Private *private;

	private = PRIVATE(button);

	printf("[btn] label_get: addr of label is %p\n", &private->label);
	return private->label;
}
