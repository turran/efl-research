#ifndef EINA_HASH_H_
#define EINA_HASH_H_

/**
 * @defgroup Hash_Group Hash
  * Hash Table Implementation:
 * 
 * Traditional hash table implementation. I had tried a list of tables
 * approach to save on the realloc's but it ended up being much slower than
 * the traditional approach.
 * @{
 */

typedef struct _eina_hash_node Eina_Hash_Node;
#define EINA_HASH_NODE(hash) ((Eina_Hash_Node *)hash)

struct _eina_hash_node {
	Eina_Hash_Node *next; /* Pointer to the next node in the bucket list */
	void *key;	     /* The key for the data node */
	void *value;	     /* The value associated with this node */
};

typedef struct _eina_hash Eina_Hash;
# define EINA_HASH(hash) ((Eina_Hash *)hash)

struct _eina_hash {
	Eina_Hash_Node **buckets;
	int size;	/* An index into the table of primes to
			 determine size */
	int nodes;	/* The number of nodes currently in the hash */

	int index;    /* The current index into the bucket table */

	Eina_Compare_Cb compare;	/* The function used to compare node values */
	Eina_Hash_Cb hash_func;	/* The callback function to determine hash */

	Eina_Free_Cb free_key;	/* The callback function to free key */
	Eina_Free_Cb free_value;	/* The callback function to free value */
};

/**
 * @defgroup Eina_Data_Hash_ADT_Creation_Group Hash Creation Functions
 *
 * Functions that create hash tables.
 * Create and initialize a hash
 * @{
 */
EAPI Eina_Hash *eina_hash_new(Eina_Hash_Cb hash_func, Eina_Compare_Cb compare);
EAPI int eina_hash_init(Eina_Hash *hash, Eina_Hash_Cb hash_func, Eina_Compare_Cb compare);
/** @} */

/**
 * @defgroup Eina_Data_Hash_ADT_Destruction_Group Hash Destruction Functions
 *
 * Functions that destroy hash tables and their contents.
 * Functions related to freeing the data in the hash table
 * @{
 */
EAPI int eina_hash_free_key_cb_set(Eina_Hash *hash, Eina_Free_Cb function);
EAPI int eina_hash_free_value_cb_set(Eina_Hash *hash, Eina_Free_Cb function);
EAPI void eina_hash_destroy(Eina_Hash *hash);
/** @} */

/**
 * @defgroup Eina_Data_Hash_ADT_Traverse_Group Hash Traverse Functions
 *
 * Functions that iterate through hash tables.
 * @{
 */

EAPI int eina_hash_count(Eina_Hash *hash);
EAPI int eina_hash_for_each_node(Eina_Hash *hash, Eina_For_Each for_each_func,
			     void *user_data);
EAPI Eina_List *eina_hash_keys(Eina_Hash *hash);
/** @} */

/**
 * @defgroup Eina_Data_Hash_ADT_Data_Group Hash Data Functions
 *
 * Functions that set, access and delete values from the hash tables.
 * Retrieve and store data into the hash
 * @{
 */

EAPI void *eina_hash_get(Eina_Hash *hash, const void *key);
EAPI int eina_hash_set(Eina_Hash *hash, void *key, void *value);
EAPI int eina_hash_hash_set(Eina_Hash *hash, Eina_Hash *set);
EAPI void *eina_hash_remove(Eina_Hash *hash, const void *key);
EAPI void *eina_hash_find(Eina_Hash *hash, Eina_Compare_Cb compare, const void *value);
EAPI void eina_hash_dump_graph(Eina_Hash *hash);
EAPI void eina_hash_dump_stats(Eina_Hash *hash);
/** @} */
/** @} */

EAPI int eina_str_compare(const void *key1, const void *key2);
EAPI int eina_direct_compare(const void *key1, const void *key2);
EAPI unsigned int eina_str_hash(const void *key);
EAPI unsigned long eina_direct_hash(const void *key);

#endif /* EINA_HASH_H */
