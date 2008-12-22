#include <stdlib.h>
#include <stdio.h>

#include "etk2_widget.h"

#define PRIVATE_OFFSET(w) ((Widget_Private*)((w) + type_size_get(object_type_get())))
#define PRIVATE(w) ((w)->private)
#define TYPE_NAME "Widget"

struct _Widget_Private
{
	int x, y, w, h;
};

static void widget_ctor(void *widget);
static void widget_dtor(void *widget);
static void widget_property_value_set(Object *object, char *prop_name, Type_Property_Value *value);
static Type_Property_Value *widget_property_value_get(Object *object, char *prop_name);

Type *widget_type_get(void)
{
	static Type *widget_type = NULL;

	if (!widget_type)
	{
		widget_type = type_new(TYPE_NAME, sizeof(Widget_Private), object_type_get(), widget_ctor, widget_dtor, widget_property_value_set, widget_property_value_get);
	}

	return widget_type;
}

Widget *widget_new(void)
{
	Widget *widget;

	widget = type_instance_new(widget_type_get());
	type_construct(widget_type_get(), widget);
	object_type_set((Object*)widget, widget_type_get());

	return widget;
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

/** Implementation **/

static void widget_ctor(void *instance)
{
	Widget *widget = (Widget*) instance;

	widget->private = PRIVATE_OFFSET(widget);
	printf("widget_ctor(%p)\n", widget);
}

static void widget_dtor(void *widget)
{
  printf("widget_dtor(%p)\n", widget);
}

static void widget_property_value_set(Object *object, char *prop_name, Type_Property_Value *value)
{
	printf("widget_prop_value_set: %s\n", prop_name);
}

static Type_Property_Value *widget_property_value_get(Object *object, char *prop_name)
{
	printf("widget_prop_value_get: %s\n", prop_name);
	return NULL;
}
