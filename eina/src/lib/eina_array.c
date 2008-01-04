#include "eina_private.h"
#include "Eina.h"

struct _Eina_Array
{
	void 	*data;
	int 	num_allocated;
	int 	num_elements;
	int 	acc;
	Eina_Array_Alloc alloc_cb;
	Eina_Array_Free free_cb;
};

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Array * eina_array_new(void *data, Eina_Array_Alloc alloc_cb, Eina_Array_Free free_cb)
{
	Eina_Array *a;

	a = calloc(1, sizeof(Eina_Array));
	a->data = data;
	a->alloc_cb = alloc_cb;
	a->free_cb = free_cb;
	return a;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eina_array_element_new(Eina_Array *a)
{
	if (a->num_elements == a->num_allocated)
	{
		a->num_allocated = (1 << a->acc);
		a->acc++;
		a->alloc_cb(a->data, a->num_allocated);
	}
	a->num_elements++;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eina_array_free(Eina_Array *a)
{
	a->free_cb(a->data);
	free(a);
}
