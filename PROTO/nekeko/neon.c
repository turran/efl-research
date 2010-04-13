#include <stdio.h>
#include <neko.h>
#include <neko_vm.h>
#include <Eon.h>
/*============================================================================*
 *                            Eon Object Interface                            *
 *============================================================================*/

DEFINE_ENTRY_POINT(eon_main);
DEFINE_KIND(k_color);

vkind k_object;

void eon_main(void)
{
	eon_init();
	kind_share(&k_object, "ekeko_object");
}

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

/* TODO add the prop name
 * do eon_value_color_from(), etc
 */
static value color_value_get(value o, value prop)
{
	Ekeko_Object *eo;
	Ekeko_Value ev;
	char *eprop;

	eprop = val_string(prop);
	eo = object_get(o);

	return val_true;
}

static value color_value_set(value o, value prop, value v)
{
	Ekeko_Object *eo;
	Ekeko_Value ev;
	char *eprop;
	value val;

	eprop = val_string(prop);
	eo = object_get(o);
	val = val_field(v, val_id("val"));
	
	eon_value_color_from(&ev, val_int(val));
	ekeko_object_property_value_set(eo, eprop, &ev);
	return val_true;
}

DEFINE_PRIM(color_value_get, 2);
DEFINE_PRIM(color_value_set, 3);

