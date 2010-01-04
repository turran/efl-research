#include "Eina.h"
#include "buddy.h"
#include <stdio.h>

int main(void)
{
	Eina_Mempool *mp;
	void *heap;
	void *data;

	eina_init();
	buddy_init();

	/* alloc the heap */
	heap = malloc(sizeof(char) * 32 * 1024 * 1024);
	if (!heap)
		return -1;
	mp = eina_mempool_add("buddy", heap, NULL, 32 * 1024 * 1024, 5);
	if (!mp)
	{
		printf("Couldnt create the buddy memory pool\n");
		free(heap);
		return -2;
	}

	data = eina_mempool_malloc(mp, 2048);
	eina_mempool_free(mp, data);
	eina_mempool_del(mp);
	free(heap);

	buddy_shutdown();
	return 0;	
}
