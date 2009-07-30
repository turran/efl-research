#include <stdio.h>
#include <neko.h>
#include <neko_vm.h>
#include <Ekeko.h>
/*============================================================================*
 *                          Ekeko Object Interface                            *
 *============================================================================*/

DEFINE_KIND(k_value);
DEFINE_KIND(k_object);
DEFINE_ENTRY_POINT(object_main);

void object_main(void)
{
	ekeko_init();
	kind_share(&k_object, "ekeko_object");
}

//val_iter_fields( value obj, void f( value v, field f, void * ), void *p );

static inline Ekeko_Object * object_get(value o)
{
	Ekeko_Object *eo;
	value inst;
	field f;
	value data = val_null;

	val_check(o, object);
	f = val_id("__inst");
	inst = val_field(o, f);
	val_check_kind(inst, k_object);
	eo = val_data(inst);
	return eo;
}

static value object_print(void)
{
	Ekeko_Object *eo;
	value t = val_this();

	eo = object_get(t);
	return alloc_string(ekeko_object_type_name_get(eo));
}

static value object_parent_get(void)
{
	Ekeko_Object *eo;
	Ekeko_Object *parent;
	value t = val_this();

	eo = object_get(t);
	parent = ekeko_object_parent_get(eo);
	if (parent)
		return alloc_abstract(k_object, parent);
	else
		return val_null;
}

static value object_next(void)
{
	Ekeko_Object *eo;
	Ekeko_Object *next;
	value t = val_this();

	eo = object_get(t);
	next = ekeko_object_next(eo);
	if (next)
		return alloc_abstract(k_object, next);
	else
		return val_null;
}

static value object_child_get_at(value index)
{
	Ekeko_Object *eo;
	Ekeko_Object *child;
	value t = val_this();

	if (!val_is_int(index))
		return val_null;

	printf("Getting at index %d\n", val_int(index));	
	eo = object_get(t);
	child = ekeko_object_child_get_at(eo, val_int(index));
	if (child)
		return alloc_abstract(k_object, child);
	else
		return val_null;
}


static value object_property_value_set(value prop, value v)
{
	value t = val_this();
	Ekeko_Value ev;
	Ekeko_Object *eo;
	char *eprop;

	eo = object_get(t);
	
	printf("SETTING PROPERTY!!!\n");
	if (!val_is_string(prop))
		goto error;

	eprop = val_string(prop);
	/* check the type */
	switch (val_type(v))
	{
		case VAL_NULL:
		goto error;
		break;

		case VAL_INT:
		ekeko_value_int_from(&ev, val_int(v));
		break;

		case VAL_STRING:
		ekeko_value_str_from(&ev, val_string(v));
		break;

		default:
		break;
	}
	ekeko_object_property_value_set(eo, eprop, &ev);
	return val_true;
error:
	return val_false;
}

static value object_property_value_get(value prop)
{
	value t = val_this();
	Ekeko_Value ev;
	value v;
	Ekeko_Object *eo;
	char *eprop;

	printf("GETTING PROPERTY!!!\n");
	eo = object_get(t);
	
	if (!val_is_string(prop))
		goto error;

	v = val_null;
	eprop = val_string(prop);
	ekeko_object_property_value_get(eo, eprop, &ev);
	/* check the type */
	switch (ev.type)
	{
		case EKEKO_PROPERTY_INT:
		v = alloc_best_int(ev.value.int_value);
		break;

		case EKEKO_PROPERTY_STRING:
		v = alloc_string(ev.value.string_value);
		break;

		case EKEKO_PROPERTY_FLOAT:
		v = alloc_float(ev.value.float_value);
		break;

		default:
		break;
	}
	return v;
error:
	return val_null;
}


static void object_delete(value v)
{
	printf("deleting the object!!\n");
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

static value object_init(void)
{
	return val_true;
}

DEFINE_PRIM(object_new, 0);
DEFINE_PRIM(object_init, 0);
DEFINE_PRIM(object_property_value_set, 2);
DEFINE_PRIM(object_property_value_get, 1);
DEFINE_PRIM(object_parent_get, 0);
DEFINE_PRIM(object_child_get_at, 1);
DEFINE_PRIM(object_next, 0);
DEFINE_PRIM(object_print, 0);
