#include "Eina.h"
#include "buddy.h"
#include <stdio.h>


size_t sizes[] = {
	2 * 1024 * 1024,
	4 * 1024 * 1024,
	8 * 1024 * 1024,
	16 * 1024,
	32 * 1024,
	48 * 1024,
	96 * 1024,
	128 * 1024,
	128 * 1024,
};

void test(Eina_Mempool *mp)
{
	char *ptr;
	int i;

	/* allocate chunks of memory, write data to them, then compare */
	for (i = 0; i < sizeof(sizes); i++)
	{
		ptr = eina_mempool_malloc(mp, 2048);
		printf("ptr = %p\n", ptr);
	}
}

int main(void)
{
	Eina_Mempool *mp;
	void *heap;
	void *data1;
	void *data2;

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
	eina_mempool_statistics(mp);
	test(mp);
	eina_mempool_statistics(mp);
	eina_mempool_del(mp);
	free(heap);

	buddy_shutdown();
	return 0;
}
