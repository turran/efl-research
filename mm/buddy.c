#include "Eina.h"
#include <stdio.h>

/*
 * This is a naive 'buddy' allocator following Knuth's documentation.
 * The main difference is that we dont store the block information
 * on the block memory itself but on another malloc'd area.
 * This is useful for managing memory which isnt as fast as the main
 * memory like the video memory
 * The algorithm uses an area to store the linked list of blocks.
 * Each block size is equal to the minimum allocatable block size for
 * the memory pool and the number of blocks is equal to the size of the
 * memory pool divided by the block size.
 */

typedef struct _Block
{
	EINA_INLIST;
	Eina_Bool available : 1;
	unsigned short int order : 7; /* final order is order + min_order */
} Block;

typedef struct _Buddy
{
	void *heap; /* start address of the heap */
	size_t size; /* total size in bytes of the heap */
	unsigned int min_order; /* minimum size is 1 << min_order */
	unsigned int max_order; /* maximum size is 1 << max_order */
	unsigned int num_order; /* number of orders */
	Eina_Inlist **areas; /* one area per order */
	Block *blocks; /* the allocated block information */
} Buddy;

/* get the minimum order greater or equal to size */
static inline unsigned int _get_order(Buddy *b, size_t size)
{
	int i;
	int bytes;

	bytes = 1 << b->min_order;
	for (i = 0; bytes < size && i < b->num_order; i++)
	{
		bytes += bytes;
	}
	//printf("order for size %d is %d\n", size, i + b->min_order);
	return i;
}

static inline void * _get_offset(Buddy *b, Block *block)
{
	void *ret;

	ret = (char *)b->heap + ((block - &b->blocks[0]) << b->min_order);
	return ret;
}

static void *_init(const char *context, const char *options, va_list args)
{
	Buddy *b;
	int i;
	int bytes;
	size_t size;
	size_t min_order;

	size = va_arg(args, int);
	min_order = va_arg(args, int);
	/* the minimum order we support is 15 (32K) */
	min_order = min_order < 15 ? 15 : min_order;
	bytes = 1 << min_order;
	for (i = 0; bytes <= size; i++)
	{
		bytes += bytes;
	}
	if (!i)
	{
		return NULL;
	}
	b = malloc(sizeof(Buddy));
	b->heap = (void *)context;
	b->size = size;
	b->min_order = min_order;
	b->max_order = min_order + i - 1;
	b->num_order = i;
	b->areas = calloc(b->num_order, sizeof(Eina_Inlist *));
	b->blocks = calloc(1 << (b->num_order - 1), sizeof(Block));
	/* setup the initial free area */
	b->blocks[0].available = EINA_TRUE;
	b->areas[b->num_order - 1] = EINA_INLIST_GET(&(b->blocks[0]));

	return b;
}

static void _shutdown(void *data)
{
	Buddy *b = data;

	free(b->blocks);
	free(b->areas);
	free(b);
}

static void _free(void *data, void *element)
{
	Buddy *b = data;
	Block *block, *buddy;
	unsigned int offset;
	unsigned int index;

	offset = element - b->heap;
	index = offset >> b->min_order;
	block = &b->blocks[index];

	printf("free %x index = %d order = %d buddy = %d\n", offset, index, block->order, index ^ (1 << block->order));
	/* we should always work with the buddy at right */
	if (index & (1 << block->order))
	{
		Block *left;

		index = index ^ (1 << block->order);
		left = &b->blocks[index];
		if (!left->available)
			goto end;
		else
		{
			buddy = block;
			block = left;
			b->areas[block->order] = eina_inlist_remove(b->areas[block->order], EINA_INLIST_GET(block));
			block->order++;
		}
	}
check:
	/* already on the last order */
	if (block->order + b->min_order == b->max_order)
		goto end;
	/* get the buddy */
	buddy = &b->blocks[index ^ (1 << block->order)];
	if (!buddy->available)
		goto end;
	/* merge two blocks */
	b->areas[block->order] = eina_inlist_remove(b->areas[block->order], EINA_INLIST_GET(buddy));
	block->order++;
	goto check;
end:
	/* add the block to the free list */
	block->available = EINA_TRUE;
	b->areas[block->order] = eina_inlist_append(b->areas[block->order], EINA_INLIST_GET(block));
}

static void *_alloc(void *data, unsigned int size)
{
	Buddy *b = data;
	Block *block, *buddy;
	int k, j;

	k = j = _get_order(b, size);
	/* get a free list of order k where k <= j <= max_order */
	while ((j < b->num_order) && !b->areas[j])
		j++;
	/* check that the order is on our range */
	if (j + b->min_order > b->max_order)
		return NULL;

	/* get a free element on this order, if not, go splitting until we find one */
found:
	if (j == k)
	{
		void *ret;

		block = EINA_INLIST_CONTAINER_GET(b->areas[j], Block);
		block->available = EINA_FALSE;
		block->order = j;
		/* remove the block from the list */
		b->areas[j] = eina_inlist_remove(b->areas[j], EINA_INLIST_GET(block));
		ret = _get_offset(b, block);
		return ret;
	}
	block = EINA_INLIST_CONTAINER_GET(b->areas[j], Block);
	/* split */
	b->areas[j] = eina_inlist_remove(b->areas[j], EINA_INLIST_GET(block));
	j--;
	b->areas[j] = eina_inlist_append(b->areas[j], EINA_INLIST_GET(block));
	buddy = block + (1 << j);
	buddy->order = j;
	buddy->available = EINA_TRUE;
	b->areas[j] = eina_inlist_append(b->areas[j], EINA_INLIST_GET(buddy));

	goto found;
}

static void *_realloc(void *data, void *element, unsigned int size)
{
	printf("realloc\n");
}

static void _garbage_collect(void *data)
{
	printf("garbage collect\n");
}

static void _statistics(void *data)
{
	Buddy *b = data;
	int i;

	printf("Information:\n");
	printf("size = %d, min_order = %d, max_order = %d, num_order = %d, num_blocks = %d (%dKB)\n", b->size, b->min_order, b->max_order, b->num_order, 1 << b->num_order, ((1 << (b->num_order)) * sizeof(Block)) / 1024);
	printf("Area dumping:");
	/* iterate over the free lists and dump the maps */
	for (i = 0; i < b->num_order; i++)
	{
		Block *block;

		printf("\n2^%d:", b->min_order + i);
		EINA_INLIST_FOREACH(b->areas[i], block)
		{
			printf(" %d", (block - &b->blocks[0]));
		}
	}
	printf("\nBlocks dumping:\n");
}

static Eina_Mempool_Backend _backend = {
	.name = "buddy",
	.init = _init,
	.free = _free,
	.alloc = _alloc,
	.shutdown = _shutdown,
	.statistics = _statistics,
};

void buddy_init(void)
{
	eina_mempool_register(&_backend);
}

void buddy_shutdown(void)
{
	eina_mempool_unregister(&_backend);
}
