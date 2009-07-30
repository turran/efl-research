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

static value color_value_get(value o)
{
	Ekeko_Object *eo;

	printf("COLOR VALUE GET!!!!!!\n");
	eo = object_get(o);
	return val_true;
}

static value color_value_set(value o, value v)
{
	Ekeko_Object *eo;

	printf("COLOR VALUE SET!!!!!!\n");
	eo = object_get(o);
	return val_true;
}

DEFINE_PRIM(color_value_get, 1);
DEFINE_PRIM(color_value_set, 1);

