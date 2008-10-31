#include "Edom.h"
#include "edom_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

struct _Edom_Element
{
	int changed;
	Eina_Hash *attributes;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Edom_Element * edom_element_new(void)
{
	Edom_Element *e;

	e = calloc(1, sizeof(Edom_Element));
	e->attributes = eina_hash_string_superfast_new(NULL);

	return e;
}

EAPI void edom_element_process(Edom_Element *e)
{
	Eina_Iterator *it;
	Edom_Attribute *a;
	
	assert(e);
	
	printf("Element changed %d\n", e->changed);
	if (!e->changed)
		return;
	
	it = eina_hash_iterator_data_new(e->attributes);
	while (eina_iterator_next(it, (void **)&a))
	{
		if (edom_attribute_update(a, e))
			e->changed--;
	}
	/* post condition */
	assert(!e->changed);
}

EAPI void edom_element_attribute_remove(Edom_Element *e, const char *name)
{
	assert(e);

	eina_hash_del(e->attributes, name, NULL);
	/* TODO delete the attribute */
}

EAPI void edom_element_attribute_add(Edom_Element *e, const char *name, Edom_Attribute_Type type,
		Edom_Value *def, Edom_Attribute_Update cb, void *data)
{
	Edom_Attribute *a;
	assert(e);

	a = edom_attribute_new(type, def, cb, data);
	eina_hash_add(e->attributes, name, a);
	
}

EAPI Eina_Bool edom_element_attribute_set(Edom_Element *e, const char *name, Edom_Value *v)
{
	Edom_Attribute *a;
	Eina_Bool changed_bef, changed_now;

	assert(e);

	a = eina_hash_find(e->attributes, name);
	if (!a) return EINA_FALSE;

	changed_bef = edom_attribute_changed(a);
	changed_now = edom_attribute_value_set(a, v);

	if (changed_bef && !changed_now)
	{
		e->changed--;
	}
	else if (!changed_bef && changed_now)
	{
		e->changed++;
	}
	if (changed_now)
		printf("Attribute %s changed now\n", name);
	if (changed_bef)
		printf("Attribute %s changed before\n", name);

	return EINA_TRUE;
}


EAPI Eina_Bool edom_element_attribute_get(Edom_Element *e, const char *name, Edom_Value *v)
{
	Edom_Attribute *a;

	assert(e);
	a = eina_hash_find(e->attributes, name);
	if (!a) return EINA_FALSE;
	
	edom_attribute_value_get(a, v);
	return EINA_TRUE;
}

#if 0
/* LATER */
Eina_List * edom_element_attributes_list(Edom_Element *e)
{
	
}


Eina_Bool edom_element_changed(Edom_Element *e)
{
	if (e->changed)
		return EINA_TRUE;
	return EINA_FALSE;
}
#endif
