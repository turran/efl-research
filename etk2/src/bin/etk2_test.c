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

	char *name;
	char *label;
	char *theme;

	// Widget tests
	widget = widget_new();
	name = "NEW WIDGET NAME";
	object_name_set((Object*)widget, name);
	printf("widget name (should be %s) = %s\n", name, object_name_get((Object*)widget));

		// Button tests
		button = button_new();
		label = "NEW BUTTON LABEL";
		theme = "GOLDEN THEME";
		button_label_set(button, label);
		object_name_set((Object*)button, name);
		widget_theme_set((Widget*)button, theme);
		printf("button's widget theme should be '%s ' and is '%s'\n", theme, widget_theme_get((Widget*)button));
		printf("button label should be '%s' = '%s', button name should be '%s' = '%s'\n", label, button_label_get(button), name, object_name_get((Object*)button));

		// Property tests
		Type_Property_Value *value = malloc(sizeof(Type_Property_Value));
		value->value.string_value = strdup("new label");

		printf("button label (should be %s) = %s\n", label, button_label_get(button));
		object_property_value_set((Object*) button, "label", value);
		printf("button label (should be 'new label') = %s\n", button_label_get(button));

		value = malloc(sizeof(Type_Property_Value));
		value->value.string_value = strdup("SILVER THEME");

		printf("button theme (should be '%s') = %s\n", theme, widget_theme_get((Widget*)button));
		object_property_value_set((Object*) button, "theme", value);
		printf("button theme (should be 'SILVER THEME') = %s\n", widget_theme_get((Widget*)button));

		return 0;
}

