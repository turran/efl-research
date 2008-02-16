#ifndef _CODE_MEMALLOC_H
#define _CODE_MEMALLOC_H

#include <stdlib.h>

void *mem_alloc(size_t nmemb, size_t size);
void mem_free(void *ptr);

#endif
