#include <stdio.h>
#include <neko.h>
#include <Ekeko.h>

DEFINE_KIND(k_object);


//val_iter_fields( value obj, void f( value v, field f, void * ), void *p );

static value object_property_value_get(void) {
	value t = val_this();
	value inst;
	field f;
	Ekeko_Value v;
	Ekeko_Object *o;

	val_check(t, object);
	f = val_id("__inst");
	inst = val_field(t, f);
	val_check_kind(inst, k_object);
	o = val_data(inst);
	ekeko_object_property_value_get(o, "id", &v);
	printf("VALUE = %s\n", v.value.string_value);

	return val_true;
}

static value object_property_value_set(void) {
	value t = val_this();
	value inst;
	field f;
	Ekeko_Value v;
	Ekeko_Object *o;

	val_check(t, object);
	f = val_id("__inst");
	inst = val_field(t, f);
	val_check_kind(inst, k_object);

	o = val_data(inst);
	ekeko_value_str_from(&v, "hola");
	ekeko_object_property_value_set(o, "id", &v);
	return val_true;
}

static void object_delete(value v)
{
	ekeko_type_instance_delete(val_data(v));
}

static value object_new(void)
{
	Ekeko_Object *o;
	value i;

	o = ekeko_object_new();
	i = alloc_abstract(k_object, o);
	val_gc(i, object_delete);

	return i;
}

DEFINE_PRIM(object_new, 0);
DEFINE_PRIM(object_property_value_set,0);
DEFINE_PRIM(object_property_value_get,0);
