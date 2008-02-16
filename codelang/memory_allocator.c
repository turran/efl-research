#include <stdlib.h>
#include <unistd.h>

#include "memory_allocator.h"

static int mem_total = 0;

void *mem_alloc(size_t nmemb, size_t size)
{
	void *mem = NULL;

	mem = calloc(nmemb, size);
	mem_total += nmemb * size;

	return mem;
}

void mem_free(void *ptr)
{
	if (ptr)
		free(ptr);
}
