#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <Etk2.h>

/* constructor / destructor test */
void test1(void)
{
	Widget *widget;

	printf("=====\n");
	printf("TEST1\n");
	printf("=====\n");
	printf("widget_new()\n");
	/* Widget API way */
	widget = widget_new();
	widget_delete(widget);
	printf("type_instance_new()\n");
	/* Type API */
	widget = type_instance_new_name_from("Widget");
	widget_delete(widget);
}

/* property set/get */
void test2(void)
{
	Type_Property_Value val;
	Widget *widget;

	printf("=====\n");
	printf("TEST2\n");
	printf("=====\n");
	widget = widget_new();
	value_str_from(&val, "GOLDEN THEME");
	object_property_value_set((Object*)widget, "theme", &val);
	value_str_from(&val, "WIDGET 1 NAME");
	object_property_value_set((Object*)widget, "name", &val);
	object_property_value_get((Object*)widget, "theme", &val);
	printf("GOLDEN_THEME = %s\n", val.value.string_value);
	object_property_value_get((Object*)widget, "name", &val);
	printf("WIDGET 1 NAME = %s\n", val.value.string_value);
}

/* event callbacks */
void prop_mod_cb(Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	printf("event type %s called prev = %s  curr = %s\n", e->type,
			em->prev.value.string_value ? em->prev.value.string_value : "NULL",
			em->curr.value.string_value ? em->curr.value.string_value : "NULL");
}

void test3(void)
{
	Type_Property_Value val;
	Widget *widget;

	printf("=====\n");
	printf("TEST3\n");
	printf("=====\n");
	widget = widget_new();
	value_str_from(&val, "GOLDEN THEME");
	event_listener_add((Object *)widget, "PropModified", prop_mod_cb);
	object_property_value_set((Object*)widget, "name", &val);
	object_name_set((Object*)widget, "SILVER THEME");
}

void testold(void)
{
	Widget *widget;
	Button *button;
	Label *l;
	Object *o;

//	int x, y, w, h;

	char *name;
	char *label;
	char *theme;

	Type_Property_Value *value;
	printf("\n\nOBJECT TESTS\n\n");
	printf("================\n");
	name = "NEW WIDGET NAME";
	label = "NEW BUTTON LABEL";
	theme = "GOLDEN THEME";

	o = object_new();
	object_name_set(o, "name!");
	printf("object_name_get(o)=%s\n", object_name_get(o));

	//printf("object_name_get(o)=%s\n", object_name_get(o));
	value = malloc(sizeof(Type_Property_Value));
	value->value.string_value = strdup("uber name");
	object_property_value_set(o, "name", value);
	printf("object_name_get(o)=%s\n", object_name_get(o));

	//return 0;

	printf("\n\nLABEL TESTS\n\n");
	printf("================\n");
	l = label_new();
	label_text_set(l, "text!");
	printf("label_text_get=%s\n", label_text_get(l));

	value = malloc(sizeof(Type_Property_Value));
	value->value.string_value = strdup("uber label");
	object_property_value_set((Object*)l, "text", value);

		value = malloc(sizeof(Type_Property_Value));
		value->value.string_value = strdup("green");
		object_property_value_set((Object*)l, "theme", value);

	//object_name_set((Object*)l, "leet!");
	printf("label_text_get=%s\n", label_text_get(l));
	printf("widget_theme_get=%s\n", widget_theme_get((Widget*)l));
	printf("label_text_get=%s\n", label_text_get(l));
	label_text_set(l, "super!");
	printf("label_text_get=%s\n", label_text_get(l));
	printf("widget_theme_get=%s\n", widget_theme_get((Widget*)l));

	//return 0;

	// Widget tests
	printf("\n\nWIDGET TESTS\n\n");
	printf("================\n");

	widget = widget_new();
	object_name_set((Object*)widget, name);
	widget_theme_set(widget, theme);
	value = malloc(sizeof(Type_Property_Value));
	value->value.string_value = strdup("FOO THEME");
	object_property_value_set((Object*)widget, "theme", value);
	printf("widget theme (should be FOO THEME) = %s\n", widget_theme_get(widget));
	printf("widget name (should be %s) = %s\n", name, object_name_get((Object*)widget));

		// Button tests
	printf("\n\nBUTTON TESTS\n\n");
	printf("================\n");

	button = button_new();

	button_label_set(button, label);
	object_name_set((Object*)button, name);
	widget_theme_set((Widget*)button, theme);

	printf("button's widget theme should be '%s ' and is '%s'\n", theme, widget_theme_get((Widget*)button));
	//printf("button label should be '%s' = '%s', button name should be '%s' = '%s'\n", label, button_label_get(button), name, object_name_get((Object*)button));

	// Property tests
	printf("\n\nPROPERTY TESTS\n\n");
	printf("================\n");

	value = malloc(sizeof(Type_Property_Value));
	value->value.string_value = strdup("new label");

	//printf("*** 4 button type's size is: %d\n", type_size_get(button_type_get()));

	//printf("button label (should be %s) = %s\n", label, button_label_get(button));
	object_property_value_set((Object*) button, "label", value);
	printf("button label (should be 'new label') = %s\n", button_label_get(button));

	value = malloc(sizeof(Type_Property_Value));
	value->value.string_value = strdup("SILVER THEME");

	//printf("button theme (should be '%s') = %s\n", theme, widget_theme_get((Widget*)button));
	object_property_value_set((Object*) button, "theme", value);
	printf("button theme (should be 'SILVER THEME') = %s\n", widget_theme_get((Widget*)button));
}



int main(int argc, char **argv)
{
	main_init();
	test1();
	test2();
	test3();
	main_shutdown();
	//testold();
	return 0;
}

