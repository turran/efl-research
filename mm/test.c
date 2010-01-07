#include "Eina.h"
#include "buddy.h"
#include <stdio.h>

int main(void)
{
	Eina_Mempool *mp;
	void *heap;
	void *data1;
	void *data2;

	eina_init();
	buddy_init();

	/* alloc the heap */
	heap = malloc(sizeof(char) * 33 * 1024 * 1024);
	if (!heap)
		return -1;
	mp = eina_mempool_add("buddy", heap, NULL, 33 * 1024 * 1024, 5);
	if (!mp)
	{
		printf("Couldnt create the buddy memory pool\n");
		free(heap);
		return -2;
	}
	eina_mempool_statistics(mp);

	data1 = eina_mempool_malloc(mp, 2048);
	printf("data = %p\n", data1);
	data2 = eina_mempool_malloc(mp, 4096);
	printf("data = %p\n", data2);
	eina_mempool_statistics(mp);
	eina_mempool_free(mp, data1);
	eina_mempool_free(mp, data2);
	eina_mempool_statistics(mp);
	eina_mempool_del(mp);
	free(heap);

	buddy_shutdown();
	return 0;	
}
