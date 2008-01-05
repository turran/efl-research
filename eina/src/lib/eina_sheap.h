#ifndef EINA_SHEAP_H_
#define EINA_SHEAP_H_

typedef struct _eina_heap Eina_Sheap;
# define EINA_HEAP(heap) ((Eina_Sheap *)heap)

struct _eina_heap {
	void **data;
	int size;
	int space;

	char order, sorted;

	/* Callback for comparing node values, default is direct comparison */
	Eina_Compare_Cb compare;

	/* Callback for freeing node data, default is NULL */
	Eina_Free_Cb free_func;
};

EAPI Eina_Sheap *eina_sheap_new(Eina_Compare_Cb compare, int size);
EAPI void eina_sheap_destroy(Eina_Sheap *heap);
EAPI int
		eina_sheap_init(Eina_Sheap *heap, Eina_Compare_Cb compare, int size);
EAPI int eina_sheap_free_cb_set(Eina_Sheap *heap, Eina_Free_Cb free_func);
EAPI int eina_sheap_insert(Eina_Sheap *heap, void *data);
EAPI void *eina_sheap_extract(Eina_Sheap *heap);
EAPI void *eina_sheap_extreme(Eina_Sheap *heap);
EAPI int eina_sheap_change(Eina_Sheap *heap, void *item, void *newval);
EAPI int eina_sheap_compare_set(Eina_Sheap *heap, Eina_Compare_Cb compare);
EAPI void eina_sheap_order_set(Eina_Sheap *heap, char order);
EAPI void eina_sheap_sort(Eina_Sheap *heap);

EAPI void *eina_sheap_item(Eina_Sheap *heap, int i);

#endif /* EINA_SHEAP_H_ */
