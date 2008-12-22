#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <Etk2.h>

int main(int argc, char **argv)
{
	Widget *widget;
	Button *button;
	int x, y, w, h;

	widget = widget_new();
	object_name_set((Object*)widget, "foo!");
	printf("widget name (should be foo) = %s\n", object_name_get((Object*)widget));
	widget_geom_set(widget, 1, 2, 3, 4);
	widget_geom_get(widget, &x, &y, &w, &h);
	printf("widget geom (should be 1 2 3 4) = %d %d %d %d\n", x, y, w, h);

	button = button_new();
	button_label_set(button, "wee!");
	object_name_set((Object*)button, "name!");
	printf("button label (should be wee!) = %s, button name (should be name!) = %s\n", button_label_get(button), object_name_get((Object*)button));

	Type_Property_Value *value = malloc(size_of(Type_Property_Value));
	value->value.string_value = strdup("new name");

	object_property_value_set((Object*) button, "name", value);
	printf("button name (should be 'new name') = %s\n", object_name_get((Object*)button));

	return 0;
}

