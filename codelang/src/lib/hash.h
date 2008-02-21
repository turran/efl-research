#ifndef _CODELANG_HASH_H
#define _CODELANG_HASH_H

typedef struct hash_s hash_t;
typedef struct hash_el_s hash_el_t;

struct hash_s
{
	int population;
	list_t *buckets[256];
};

struct hash_el_s
{
	list_t _list_data;
	const char *key;
	void *data;
};

#endif
