#include "Codelang.h"

static inline int _hash_gen(const char *key);

static int _hash_alloc_error = 0;

static inline int
_hash_gen(const char *key)
{
   unsigned int hash_num = 5381;
   const unsigned char *ptr;
   
   if (!key) return 0;
   for (ptr = (unsigned char *)key; *ptr; ptr++)
     hash_num = (hash_num * 33) ^ *ptr;
   
   hash_num &= 0xff;
   return (int)hash_num;
}

hash_t *
hash_add(hash_t *hash, const char *key, const void *data)
{
   int hash_num;
   hash_t_El *el;

   if ((!key) || (!data)) return hash;
   _hash_alloc_error = 0;
   if (!hash)
     {
	hash = calloc(1, sizeof(struct _hash_t));
	if (!hash)
	  {
	     _hash_alloc_error = 1;
	     return NULL;
	  }
     }
   if (!(el = malloc(sizeof(struct _hash_t_El) + strlen(key) + 1)))
     {
        if (hash->population <= 0)
	  {
	     free(hash);
	     hash = NULL;
	  }
	_hash_alloc_error = 1;
	return hash;
     };
   el->key = ((char *)el) + sizeof(struct _hash_t_El);
   strcpy((char *) el->key, key);
   el->data = (void *)data;
   hash_num = _hash_gen(key);
   hash->buckets[hash_num] = list_prepend(hash->buckets[hash_num], el);
   if (evas_list_alloc_error())
     {
	_hash_alloc_error = 1;
	free(el);
	return hash;
     }
   hash->population++;
   return hash;
}

hash_t *
hash_direct_add(hash_t *hash, const char *key, const void *data)
{
   int hash_num;
   hash_t_El *el;

   if ((!key) || (!data)) return hash;
   _hash_alloc_error = 0;
   if (!hash)
     {
	hash = calloc(1, sizeof(struct _hash_t));
	if (!hash)
	  {
	     _hash_alloc_error = 1;
	     return NULL;
	  }
     }
   if (!(el = malloc(sizeof(struct _hash_t_El))))
     {
        if (hash->population <= 0)
	  {
	     free(hash);
	     hash = NULL;
	  }
	_hash_alloc_error = 1;
	return hash;
     };
   el->key = key;
   el->data = (void *)data;
   hash_num = _hash_gen(key);
   hash->buckets[hash_num] = list_prepend(hash->buckets[hash_num], el);
   if (evas_list_alloc_error())
     {
	_hash_alloc_error = 1;
	free(el);
	return hash;
     }
   hash->population++;
   return hash;
}

hash_t *
hash_del(hash_t *hash, const char *key, const void *data)
{
   int hash_num;
   hash_t_El *el;
   list_t *l;

   if (!hash) return NULL;
   if (!key)
     {
	int hash_num;
	
	for (hash_num = 0; hash_num < 256; hash_num++)
	  {
	     for (l = hash->buckets[hash_num]; l; l = l->next)
	       {
		  el = (hash_t_El *)l;
		  if (el->data == data)
		    {
		       hash->buckets[hash_num] = list_remove(hash->buckets[hash_num], el);
		       free(el);
		       hash->population--;
		       if (hash->population <= 0)
			 {
			    free(hash);
			    hash = NULL;
			 }
		       return hash;
		    }
	       }
	  }
     }
   else
     {
	hash_num = _hash_gen(key);
	for (l = hash->buckets[hash_num]; l; l = l->next)
	  {
	     el = (hash_t_El *)l;
	     if (!strcmp(el->key, key))
	       {
		  hash->buckets[hash_num] = list_remove(hash->buckets[hash_num], el);
		  free(el);
		  hash->population--;
		  if (hash->population <= 0)
		    {
		       free(hash);
		       hash = NULL;
		    }
		  return hash;
	       }
	  }
     }
   return hash;
}

void *
hash_find(const hash_t *hash, const char *key)
{
   int hash_num;
   hash_t_El *el;
   list_t *l;

   _hash_alloc_error = 0;
   if ((!hash) || (!key)) return NULL;
   hash_num = _hash_gen(key);
   for (l = hash->buckets[hash_num]; l; l = l->next)
     {
	el = (hash_t_El *)l;
	if (!strcmp(el->key, key))
	  {
	     if (l != hash->buckets[hash_num])
	       {
		  list_t *bucket;

		  bucket = hash->buckets[hash_num];
		  bucket = list_remove(bucket, el);
		  bucket = list_prepend(bucket, el);
		  ((hash_t *)hash)->buckets[hash_num] = bucket;
	       }
	     return el->data;
	  }
     }
   return NULL;
}

void *
hash_modify(hash_t *hash, const char *key, const void *data)
{
   int hash_num;
   hash_t_El *el;
   list_t *l;

   _hash_alloc_error = 0;
   if (!hash) return NULL;
   hash_num = _hash_gen(key);
   for (l = hash->buckets[hash_num]; l; l = l->next)
     {
	el = (hash_t_El *)l;
	if ((key) && (!strcmp(el->key, key)))
	  {
	     void *old_data;
	     
	     if (l != hash->buckets[hash_num])
	       {
		  hash->buckets[hash_num] = list_remove(hash->buckets[hash_num], el);
		  hash->buckets[hash_num] = list_prepend(hash->buckets[hash_num], el);
	       }
	     old_data = el->data;
	     el->data = (void *) data;
	     return old_data;
	  }
     }
   return NULL;
}

int
hash_size(const hash_t *hash)
{
   if (!hash) return 0;
   return 256;
}

void
hash_free(hash_t *hash)
{
   int i, size;

   if (!hash) return;
   size = hash_size(hash);
   for (i = 0; i < size; i++)
     {
	while (hash->buckets[i])
	  {
	     hash_t_El *el;

	     el = (hash_t_El *)hash->buckets[i];
	     hash->buckets[i] = list_remove(hash->buckets[i], el);
	     free(el);
	  }
     }
   free(hash);
}

void
hash_foreach(const hash_t *hash, Evas_Bool (*func) (const hash_t *hash, const char *key, void *data, void *fdata), const void *fdata)
{
   int i, size;

   if (!hash) return;
   size = hash_size(hash);
   for (i = 0; i < size; i++)
     {
	list_t *l, *next_l;

	for (l = hash->buckets[i]; l;)
	  {
	     hash_t_El *el;

	     next_l = l->next;
	     el = (hash_t_El *)l;
	     if (!func(hash, el->key, el->data, (void *)fdata)) return;
	     l = next_l;
	  }
     }
}

int
hash_alloc_error(void)
{
   return _hash_alloc_error;
}
