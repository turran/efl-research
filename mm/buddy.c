#include "Eina.h"
#include <stdio.h>

typedef struct _Buddy_Area
{
	EINA_INLIST;
	unsigned long *map;
} Buddy_Area;

typedef struct _Buddy
{
	void *heap; /* start address of the heap */
	size_t size; /* total size in bytes of the heap */
	size_t min_order; /* minimum size is 1 << min_order */
	unsigned int num_order; /* number of orders */
	Buddy_Area *areas;
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
	printf("order for size %d is %d\n", size, i + b->min_order);
}

static inline void _coalesce(Buddy *b)
{

}

static inline void _split(Buddy *b)
{

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

	bytes = 1 << min_order;
	for (i = 0; bytes <= size; i++)
	{
		bytes += bytes;
	}
	if (!i)
	{
		printf("error\n");
		return NULL;
	}
	b = malloc(sizeof(Buddy));
	b->heap = (void *)context;
	b->size = size;
	b->min_order = min_order;
	b->num_order = i - 1;
	b->areas = calloc(sizeof(Buddy_Area) * b->num_order);

	printf("size = %d, min_order = %d, max_order = %d\n", b->size, b->min_order, b->min_order + b->num_order);
	return b;
}

static void _shutdown(void *data)
{
	Buddy *b = data;

	free(b);
}

static void _free(void *data, void *element)
{
	printf("free\n");
}

static void *_alloc(void *data, unsigned int size)
{
	Buddy *b = data;
	int order;

	order = _get_order(b, size);
	/* get a free element on this order, if not go splitting until we find one */
	

	return NULL;
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
	printf("stats\n");
	/* iterate over the free lists and dump the maps */
}

static Eina_Mempool_Backend _backend = {
	.name = "buddy",
	.init = _init,
	.free = _free,
	.alloc = _alloc,
	.shutdown = _shutdown,
};

void buddy_init(void)
{
	eina_mempool_register(&_backend);
}

void buddy_shutdown(void)
{
	eina_mempool_unregister(&_backend);
}
