#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(w) ((w)->private)
#define TYPE_NAME "Widget"

struct _Widget_Private
{
	int x, y, w, h;
	char *theme;
};

static Eina_Bool _appendable(const char *type)
{
	/* TODO only allow some object's to be appendable */
	printf("appendable %s?\n", type);
	return EINA_TRUE;
}

static void _ctor(void *instance)
{
	Widget *widget = (Widget*) instance;

	widget->private = type_instance_private_get(widget_type_get(), instance);
	widget->private->theme = NULL;
	((Container *)widget)->appendable = _appendable;
	printf("[widget] ctor %p %p\n", widget, widget->private);
}

static void _dtor(void *widget)
{
  printf("[widget] dtor %p\n", widget);
}

static void widget_property_value_set(Object *object, char *prop_name, Value *value)
{
	printf("widget_prop_value_set: %s\n", prop_name);
}

static Value *widget_property_value_get(Object *object, char *prop_name)
{
	printf("widget_prop_value_get: %s\n", prop_name);
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *widget_type_get(void)
{
	static Type *widget_type = NULL;

	if (!widget_type)
	{
		widget_type = type_new(TYPE_NAME, sizeof(Widget), sizeof(Widget_Private), container_type_get(), _ctor, _dtor, widget_property_value_set, widget_property_value_get);
		type_property_new(widget_type, "theme", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Widget_Private, theme), NULL);
	}

	return widget_type;
}

Widget *widget_new(void)
{
	Widget *widget;

	widget = type_instance_new(widget_type_get());

	return widget;
}

void widget_delete(Widget *w)
{
	type_instance_delete(w);
}

void widget_geom_set(Widget *widget, int x, int y, int w, int h)
{
	Widget_Private *private;

	private = PRIVATE(widget);
	private->x = x;
	private->y = y;
	private->w = w;
	private->h = h;
}

void widget_geom_get(Widget *widget, int *x, int *y, int *w, int *h)
{
	Widget_Private *private;

	private  = PRIVATE(widget);

	if (x) *x = private->x;
	if (y) *y = private->y;
	if (w) *w = private->w;
	if (h) *h = private->h;
}

void widget_theme_set(Widget *widget, char *theme)
{
	Value value;

	value_str_from(&value, theme);
	object_property_value_set((Object *)widget, "theme", &value);
}

char *widget_theme_get(Widget *widget)
{
	Widget_Private *private;

	RETURN_NULL_IF(widget == NULL);

	private = PRIVATE(widget);
	return private->theme;
}
