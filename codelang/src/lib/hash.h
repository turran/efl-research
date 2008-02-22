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

hash_t *hash_add(hash_t *hash, const char *key, const void *data);
hash_t *hash_direct_add(hash_t *hash, const char *key, const void *data);
hash_t *hash_del(hash_t *hash, const char *key, const void *data);
void *hash_find(hash_t *hash, const char *key);
void *hash_modify(hash_t *hash, const char *key, const void *data);
int hash_size(hash_t *hash);
void hash_free(hash_t *hash);

#endif
