#include "Edom.h"
#include "edom_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Edom_Attribute
{
	Edom_Attribute_Type type;
	Edom_Value curr;
	Edom_Value prev;
	Edom_Attribute_Update cb;
	void *cb_data;
	Eina_Bool changed;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Edom_Attribute * edom_attribute_new(Edom_Attribute_Type type, Edom_Value *def,
		Edom_Attribute_Update cb, void *data)
{
	Edom_Attribute *a;
	
	a = calloc(1, sizeof(Edom_Attribute));
	a->cb = cb;
	a->type = type;
	a->cb_data = data;
	a->changed = EINA_FALSE;
	if (def)
		a->curr = a->prev = *def;
	
	return a;
}
Eina_Bool edom_attribute_changed(Edom_Attribute *a)
{
	return a->changed;
}
Eina_Bool edom_attribute_update(Edom_Attribute *a, Edom_Element *e)
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

Eina_Bool edom_attribute_value_set(Edom_Attribute *a, Edom_Value *v)
{
	Eina_Bool changed = EINA_FALSE;
	
	assert(a->type == v->type);

	switch (a->curr.type)
	{
	/* First basic types */
	case EDOM_ATTRIBUTE_INT:
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

void edom_attribute_value_get(Edom_Attribute *a, Edom_Value *v)
{
	*v = a->curr;
}
