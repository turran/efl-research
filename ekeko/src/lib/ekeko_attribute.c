#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Attribute
{
	Ekeko_Value_Type type;
	Ekeko_Value curr;
	Ekeko_Value prev;
	Ekeko_Element_Update cb;
	void *cb_data;
	Eina_Bool changed;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Attribute * ekeko_attribute_new(Ekeko_Value_Type type, Ekeko_Value *def,
		Ekeko_Element_Update cb, void *data)
{
	Ekeko_Attribute *a;

	a = calloc(1, sizeof(Ekeko_Attribute));
	a->cb = cb;
	a->type = type;
	a->cb_data = data;
	a->changed = EINA_FALSE;
	if (def)
		a->curr = a->prev = *def;

	return a;
}
Eina_Bool ekeko_attribute_changed(Ekeko_Attribute *a)
{
	return a->changed;
}
Eina_Bool ekeko_attribute_update(Ekeko_Attribute *a, Ekeko_Element *e)
{
	if (!a->changed)
		return EINA_FALSE;
	/* update the attribute */
	if (a->cb)
	{
		a->cb(e, &a->curr, &a->prev, a->cb_data);
	}
	return EINA_TRUE;
}

Eina_Bool ekeko_attribute_value_set(Ekeko_Attribute *a, Ekeko_Value *v)
{
	Eina_Bool changed = EINA_FALSE;

	assert(a->type == v->type);

	switch (a->curr.type)
	{
	/* First basic types */
	case EKEKO_ATTRIBUTE_INT:
		if (v->v.i != a->prev.v.i)
		{
			changed = EINA_TRUE;
			a->curr.v.i = v->v.i;
		}
		break;
	/* TODO User provided ones */
	default:
		break;
	}

	a->changed = changed;
	return changed;
}

void ekeko_attribute_value_get(Ekeko_Attribute *a, Ekeko_Value *v)
{
	*v = a->curr;
}
