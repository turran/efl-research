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
	Value val;
	Widget *widget;

	printf("=====\n");
	printf("TEST2\n");
	printf("=====\n");
	widget = widget_new();
	value_str_from(&val, "GOLDEN THEME");
	object_property_value_set((Object*)widget, "theme", &val);
	value_str_from(&val, "WIDGET 1 NAME");
	object_property_value_set((Object*)widget, "id", &val);
	object_property_value_get((Object*)widget, "theme", &val);
	printf("GOLDEN_THEME = %s\n", val.value.string_value);
	object_property_value_get((Object*)widget, "id", &val);
	printf("WIDGET 1 NAME = %s\n", val.value.string_value);
}

/* event callbacks */
void prop_mod_cb(Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	printf("event type %s (%s-%d) called prev = %s  curr = %s\n", e->type,
			em->prop, em->prop_id,
			em->prev.value.string_value ? em->prev.value.string_value : "NULL",
			em->curr.value.string_value ? em->curr.value.string_value : "NULL");
}

void test3(void)
{
	Value val;
	Widget *widget;

	printf("=====\n");
	printf("TEST3\n");
	printf("=====\n");
	widget = widget_new();
	value_str_from(&val, "GOLDEN THEME");
	event_listener_add((Object *)widget, EVENT_MUTATION_PROPMODIFY, prop_mod_cb);
	object_property_value_set((Object*)widget, "id", &val);
	object_id_set((Object*)widget, "SILVER THEME");
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

