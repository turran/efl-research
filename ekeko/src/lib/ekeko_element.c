#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

struct _Ekeko_Element
{
	int changed;
	Eina_Hash *attributes;
#ifdef EKEKO_EVENT
	Eina_Hash *events;
#endif
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Element * ekeko_element_new(void)
{
	Ekeko_Element *e;

	e = calloc(1, sizeof(Ekeko_Element));
	e->attributes = eina_hash_string_superfast_new(NULL);

	return e;
}

EAPI Ekeko_Element * ekeko_element_new_ns(const char *ns)
{

}

EAPI void ekeko_element_process(Ekeko_Element *e)
{
	Eina_Iterator *it;
	Ekeko_Attribute *a;
	
	assert(e);
	
	printf("Element changed %d\n", e->changed);
	if (!e->changed)
		return;
	
	it = eina_hash_iterator_data_new(e->attributes);
	while (eina_iterator_next(it, (void **)&a))
	{
		if (ekeko_attribute_update(a, e))
			e->changed--;
	}
	/* post condition */
	assert(!e->changed);
}

EAPI void ekeko_element_attribute_remove(Ekeko_Element *e, const char *name)
{
	assert(e);

	eina_hash_del(e->attributes, name, NULL);
	/* TODO delete the attribute */
}

EAPI void ekeko_element_attribute_add(Ekeko_Element *e, const char *name, Ekeko_Value_Type type,
		Ekeko_Value *def, Ekeko_Element_Update cb, void *data)
{
	Ekeko_Attribute *a;
	assert(e);

	a = ekeko_attribute_new(type, def, cb, data);
	eina_hash_add(e->attributes, name, a);
	
}

EAPI Eina_Bool ekeko_element_attribute_set(Ekeko_Element *e, const char *name, Ekeko_Value *v)
{
	Ekeko_Attribute *a;
	Eina_Bool changed_bef, changed_now;

	assert(e);

	a = eina_hash_find(e->attributes, name);
	if (!a) return EINA_FALSE;

	changed_bef = ekeko_attribute_changed(a);
	changed_now = ekeko_attribute_value_set(a, v);

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


EAPI Eina_Bool ekeko_element_attribute_get(Ekeko_Element *e, const char *name, Ekeko_Value *v)
{
	Ekeko_Attribute *a;

	assert(e);
	a = eina_hash_find(e->attributes, name);
	if (!a) return EINA_FALSE;
	
	ekeko_attribute_value_get(a, v);
	return EINA_TRUE;
}

#if 0
/* LATER */
Eina_List * ekeko_element_attributes_list(Ekeko_Element *e)
{
	
}


Eina_Bool ekeko_element_changed(Ekeko_Element *e)
{
	if (e->changed)
		return EINA_TRUE;
	return EINA_FALSE;
}
#endif
