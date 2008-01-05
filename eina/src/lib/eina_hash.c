#include "Eina.h"
#include "eina_private.h"

#define PRIME_TABLE_MAX 21
#define PRIME_MIN 17
#define PRIME_MAX 16777213

#define EINA_HASH_CHAIN_MAX 3

#define EINA_COMPUTE_HASH(hash, key) hash->hash_func(key) % \
					eina_prime_table[hash->size];

#define EINA_HASH_INCREASE(hash) ((hash && eina_prime_table[hash->size] < PRIME_MAX) ? \
		(hash->nodes / eina_prime_table[hash->size]) > \
		EINA_HASH_CHAIN_MAX : FALSE)
#define EINA_HASH_REDUCE(hash) ((hash && eina_prime_table[hash->size] > PRIME_MIN) ? \
		(double)hash->nodes / (double)eina_prime_table[hash->size-1] \
		< ((double)EINA_HASH_CHAIN_MAX * 0.375) : FALSE)

/* Private hash manipulation functions */
static int _eina_hash_add_node(Eina_Hash *hash, Eina_Hash_Node *node);
static Eina_Hash_Node * _eina_hash_get_node(Eina_Hash *hash, const void *key);
static int _eina_hash_increase(Eina_Hash *hash);
static int _eina_hash_decrease(Eina_Hash *hash);
inline int _eina_hash_rehash(Eina_Hash *hash, Eina_Hash_Node **old_table, int old_size);
static int _eina_hash_bucket_destroy(Eina_Hash_Node *list, Eina_Free_Cb keyd, Eina_Free_Cb valued);
inline Eina_Hash_Node * _eina_hash_get_bucket(Eina_Hash *hash, Eina_Hash_Node *bucket, const void *key);
static Eina_Hash_Node *_eina_hash_node_new(void *key, void *value);
static int _eina_hash_node_init(Eina_Hash_Node *node, void *key, void *value);
static int _eina_hash_node_destroy(Eina_Hash_Node *node, Eina_Free_Cb keyd, Eina_Free_Cb valued);

/**
 * Creates and initializes a new hash
 * @param hash_func The function for determining hash position.
 * @param compare   The function for comparing node keys.
 * @return @c NULL on error, a new hash on success.
 */
EAPI Eina_Hash *
eina_hash_new(Eina_Hash_Cb hash_func, Eina_Compare_Cb compare)
{
	Eina_Hash *new_hash = (Eina_Hash *)malloc(sizeof(Eina_Hash));
	if (!new_hash)
	return NULL;

	if (!eina_hash_init(new_hash, hash_func, compare))
	{
		FREE(new_hash);
		return NULL;
	}

	return new_hash;
}

/**
 * Initializes the given hash.
 * @param   hash       The given hash.
 * @param   hash_func  The function used for hashing node keys.
 * @param   compare    The function used for comparing node keys.
 * @return  @c TRUE on success, @c FALSE on an error.
 */
EAPI int eina_hash_init(Eina_Hash *hash, Eina_Hash_Cb hash_func,
		Eina_Compare_Cb compare)
{
	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	memset(hash, 0, sizeof(Eina_Hash));

	hash->hash_func = hash_func;
	hash->compare = compare;

	hash->buckets = (Eina_Hash_Node **)calloc(eina_prime_table[0],
			sizeof(Eina_Hash_Node *));

	return TRUE;
}

/**
 * Sets the function to destroy the keys of the given hash.
 * @param   hash     The given hash.
 * @param   function The function used to free the node keys. NULL is a
 *          valid value and means that no function will be called.
 * @return  @c TRUE on success, @c FALSE on error.
 */
EAPI int eina_hash_free_key_cb_set(Eina_Hash *hash, Eina_Free_Cb function)
{
	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	hash->free_key = function;

	return TRUE;
}

/**
 * Sets the function to destroy the values in the given hash.
 * @param   hash     The given hash.
 * @param   function The function that will free the node values. NULL is a
 *          valid value and means that no function will be called.
 * @return  @c TRUE on success, @c FALSE on error
 */
EAPI int eina_hash_free_value_cb_set(Eina_Hash *hash, Eina_Free_Cb function)
{
	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	hash->free_value = function;

	return TRUE;
}

/**
 * Sets a key-value pair in the given hash table.
 * @param   hash    The given hash table.
 * @param   key     The key.
 * @param   value   The value.
 * @return  @c TRUE if successful, @c FALSE if not.
 */
EAPI int eina_hash_set(Eina_Hash *hash, void *key, void *value)
{
	int ret = FALSE;
	Eina_Hash_Node *node;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	node = _eina_hash_get_node(hash, key);
	if (node)
	{
		if (hash->free_key)
			hash->free_key(key);
		if (node->value && hash->free_value)
			hash->free_value(node->value);
		node->value = value;
		ret = TRUE;
	}
	else
	{
		node = _eina_hash_node_new(key, value);
		if (node)
			ret = _eina_hash_add_node(hash, node);
	}

	return ret;
}

/**
 * Sets all key-value pairs from set in the given hash table.
 * @param   hash    The given hash table.
 * @param   set     The hash table to import.
 * @return  @c TRUE if successful, @c FALSE if not.
 */
EAPI int eina_hash_hash_set(Eina_Hash *hash, Eina_Hash *set)
{
	unsigned int i;
	Eina_Hash_Node *node, *old;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);
	CHECK_PARAM_POINTER_RETURN("set", set, FALSE);

	for (i = 0; i < eina_prime_table[set->size]; i++)
	{
		/* Hash into a new list to avoid loops of rehashing the same nodes */
		while ((old = set->buckets[i]))
		{
			set->buckets[i] = old->next;
			old->next = NULL;
			node = _eina_hash_get_node(hash, old->key);
			if (node)
			{
				/* This key already exists. Delete the old and add the new
				 * value */
				if (hash->free_key)
					hash->free_key(node->key);
				if (hash->free_value)
					hash->free_key(node->value);
				node->key = old->key;
				node->value = old->value;
				free(old);
			}
			else
				_eina_hash_add_node(hash, old);
		}
	}
	FREE(set->buckets);
	eina_hash_init(set, set->hash_func, set->compare);
	return TRUE;
}

/**
 * Frees the hash table and the data contained inside it.
 * @param   hash The hash table to destroy.
 * @return  @c TRUE on success, @c FALSE on error.
 */
EAPI void eina_hash_destroy(Eina_Hash *hash)
{
	unsigned int i = 0;

	CHECK_PARAM_POINTER("hash", hash);

	if (hash->buckets)
	{
		while (i < eina_prime_table[hash->size])
		{
			if (hash->buckets[i])
			{
				Eina_Hash_Node *bucket;

				/*
				 * Remove the bucket list to avoid possible recursion
				 * on the free callbacks.
				 */
				bucket = hash->buckets[i];
				hash->buckets[i] = NULL;
				_eina_hash_bucket_destroy(bucket, hash->free_key,
						hash->free_value);
			}
			i++;
		}

		FREE(hash->buckets);
	}
	FREE(hash);

	return;
}

/**
 * Counts the number of nodes in a hash table.
 * @param   hash The hash table to count current nodes.
 * @return  The number of nodes in the hash.
 */
EAPI int eina_hash_count(Eina_Hash *hash)
{
	CHECK_PARAM_POINTER_RETURN("hash", hash, 0);

	return hash->nodes;
}

/**
 * Runs the @p for_each_func function on each entry in the given hash.
 * @param   hash          The given hash.
 * @param   for_each_func The function that each entry is passed to.
 * @param		user_data			a pointer passed to calls of for_each_func
 * @return  TRUE on success, FALSE otherwise.
 */
EAPI int eina_hash_for_each_node(Eina_Hash *hash,
		Eina_For_Each for_each_func, void *user_data)
{
	unsigned int i = 0;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);
	CHECK_PARAM_POINTER_RETURN("for_each_func", for_each_func, FALSE);

	while (i < eina_prime_table[hash->size])
	{
		if (hash->buckets[i])
		{
			Eina_Hash_Node *node;

			for (node = hash->buckets[i]; node; node = node->next)
			{
				for_each_func(node, user_data);
			}
		}
		i++;
	}

	return TRUE;
}

/**
 * Retrieves an eina_list of all keys in the given hash.
 * @param   hash          The given hash.
 * @return  new eina_list on success, NULL otherwise
 */
EAPI Eina_List *
eina_hash_keys(Eina_Hash *hash)
{
	unsigned int i = 0;
	Eina_List *keys;

	CHECK_PARAM_POINTER_RETURN("hash", hash, NULL);

	keys = eina_list_new();
	while (i < eina_prime_table[hash->size])
	{
		if (hash->buckets[i])
		{
			Eina_Hash_Node *node;

			for (node = hash->buckets[i]; node; node = node->next)
			{
				eina_list_append(keys, node->key);
			}
		}
		i++;
	}
	eina_list_first_goto(keys);

	return keys;
}

/**
 * Prints the distribution of the given hash table for graphing.
 * @param hash The given hash table.
 */
EAPI void eina_hash_dump_graph(Eina_Hash *hash)
{
	unsigned int i;

	for (i = 0; i < eina_prime_table[hash->size]; i++)
		if (hash->buckets[i])
		{
			int n = 0;
			Eina_Hash_Node *node;
			for (node = hash->buckets[i]; node; node = node->next)
				n++;
			printf("%d\t%u\n", i, n);
		}
		else
			printf("%d\t0\n", i);
}

/**
 * Prints the distribution of the given hash table for graphing.
 * @param hash The given hash table.
 */
EAPI void eina_hash_dump_stats(Eina_Hash *hash)
{
	unsigned int i;
	double variance, sum_n_2 = 0, sum_n = 0;

	for (i = 0; i < eina_prime_table[hash->size]; i++)
	{
		if (hash->buckets[i])
		{
			int n = 0;
			Eina_Hash_Node *node;
			for (node = hash->buckets[i]; node; node = node->next)
				n++;
			sum_n_2 += ((double)n * (double)n);
			sum_n += (double)n;
		}
	}
	variance = (sum_n_2 - ((sum_n * sum_n) / (double)i)) / (double)i;
	printf("Average length: %f\n\tvariance^2: %f\n", (sum_n / (double)i),
			variance);
}

static int _eina_hash_bucket_destroy(Eina_Hash_Node *list,
		Eina_Free_Cb keyd, Eina_Free_Cb valued)
{
	Eina_Hash_Node *node;

	CHECK_PARAM_POINTER_RETURN("list", list, FALSE);

	for (node = list; node; node = list)
	{
		list = list->next;
		_eina_hash_node_destroy(node, keyd, valued);
	}

	return TRUE;
}

/*
 * @brief Add the node to the hash table
 * @param hash: the hash table to add the key
 * @param node: the node to add to the hash table
 * @return Returns FALSE on error, TRUE on success
 */
static int _eina_hash_add_node(Eina_Hash *hash, Eina_Hash_Node *node)
{
	unsigned int hash_val;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);
	CHECK_PARAM_POINTER_RETURN("node", node, FALSE);

	/* Check to see if the hash needs to be resized */
	if (EINA_HASH_INCREASE(hash))
		_eina_hash_increase(hash);

	/* Compute the position in the table */
	if (!hash->hash_func)
		hash_val = (unsigned int)node->key % eina_prime_table[hash->size];
	else
		hash_val = EINA_COMPUTE_HASH(hash, node->key);

	/* Prepend the node to the list at the index position */
	node->next = hash->buckets[hash_val];
	hash->buckets[hash_val] = node;
	hash->nodes++;

	return TRUE;
}

/**
 * Retrieves the value associated with the given key from the given hash
 * table.
 * @param   hash The given hash table.
 * @param   key  The key to search for.
 * @return  The value corresponding to key on success, @c NULL otherwise.
 */
EAPI void * eina_hash_get(Eina_Hash *hash, const void *key)
{
	void *data;
	Eina_Hash_Node *node;

	CHECK_PARAM_POINTER_RETURN("hash", hash, NULL);

	node = _eina_hash_get_node(hash, key);
	if (!node)
		return NULL;

	data = node->value;

	return data;
}

/**
 * Removes the value associated with the given key in the given hash
 * table.
 * @param   hash The given hash table.
 * @param   key  The key to search for.
 * @return  The value corresponding to the key on success.  @c NULL is
 *          returned if there is an error.
 */
EAPI void * eina_hash_remove(Eina_Hash *hash, const void *key)
{
	Eina_Hash_Node *node = NULL;
	Eina_Hash_Node *list;
	unsigned int hash_val;
	void *ret = NULL;

	CHECK_PARAM_POINTER_RETURN("hash", hash, NULL);

	/* Compute the position in the table */
	if (!hash->hash_func)
		hash_val = (unsigned int )key % eina_prime_table[hash->size];
	else
		hash_val = EINA_COMPUTE_HASH(hash, key);

	/*
	 * If their is a list that could possibly hold the key/value pair
	 * traverse it and remove the hash node.
	 */
	if (hash->buckets[hash_val])
	{
		list = hash->buckets[hash_val];

		/*
		 * Traverse the list to find the specified key
		 */
		node = list;
		if (hash->compare)
		{
			while ((node) && (hash->compare(node->key, key) != 0))
			{
				list = node;
				node = node->next;
			}
		}
		else
		{
			while ((node) && (node->key != key))
			{
				list = node;
				node = node->next;
			}
		}

		/*
		 * Remove the node with the matching key and free it's memory
		 */
		if (node)
		{
			if (list == node)
				hash->buckets[hash_val] = node->next;
			else
				list->next = node->next;
			ret = node->value;
			node->value = NULL;
			_eina_hash_node_destroy(node, hash->free_key, NULL);
			hash->nodes--;
		}
	}

	if (EINA_HASH_REDUCE(hash))
		_eina_hash_decrease(hash);

	return ret;
}

/**
 * Retrieves the first value that matches
 * table.
 * @param   hash The given hash table.
 * @param   key  The key to search for.
 * @return  The value corresponding to key on success, @c NULL otherwise.
 */
EAPI void * eina_hash_find(Eina_Hash *hash, Eina_Compare_Cb compare,
		const void *value)
{
	unsigned int i = 0;

	CHECK_PARAM_POINTER_RETURN("hash", hash, NULL);
	CHECK_PARAM_POINTER_RETURN("compare", compare, NULL);
	CHECK_PARAM_POINTER_RETURN("value", value, NULL);

	while (i < eina_prime_table[hash->size])
	{
		if (hash->buckets[i])
		{
			Eina_Hash_Node *node;

			for (node = hash->buckets[i]; node; node = node->next)
			{
				if (!compare(node->value, value))
					return node->value;
			}
		}
		i++;
	}

	return NULL;
}

/*
 * @brief Retrieve the node associated with key
 * @param hash: the hash table to search for the key
 * @param key: the key to search for in the hash table
 * @return Returns NULL on error, node corresponding to key on success
 */
static Eina_Hash_Node * _eina_hash_get_node(Eina_Hash *hash, const void *key)
{
	unsigned int hash_val;
	Eina_Hash_Node *node = NULL;

	CHECK_PARAM_POINTER_RETURN("hash", hash, NULL);

	if (!hash->buckets)
	{
		return NULL;
	}

	/* Compute the position in the table */
	if (!hash->hash_func)
		hash_val = (unsigned int )key % eina_prime_table[hash->size];
	else
		hash_val = EINA_COMPUTE_HASH(hash, key);

	/* Grab the bucket at the specified position */
	if (hash->buckets[hash_val])
	{
		node = _eina_hash_get_bucket(hash, hash->buckets[hash_val], key);
		/*
		 * Move matched node to the front of the list as it's likely
		 * to be searched for again soon.
		 */
		if (node && node != hash->buckets[hash_val])
		{
			node->next = hash->buckets[hash_val];
			hash->buckets[hash_val] = node;
		}
	}

	return node;
}

/*
 * @brief Search the hash bucket for a specified key
 * @param hash: the hash table to retrieve the comparison function
 * @param bucket: the list to search for the key
 * @param key: the key to search for in the list
 * @return Returns NULL on error or not found, the found node on success
 */
inline Eina_Hash_Node * _eina_hash_get_bucket(Eina_Hash *hash,
		Eina_Hash_Node *bucket, const void *key)
{
	Eina_Hash_Node *prev = NULL;
	Eina_Hash_Node *node = NULL;

	/*
	 * Traverse the list to find the desired node, if the node is in the
	 * list, then return the node.
	 */
	if (hash->compare)
	{
		for (node = bucket; node; node = node->next)
		{
			if (hash->compare(node->key, key) == 0)
				break;
			prev = node;
		}
	}
	else
	{
		for (node = bucket; node; node = node->next)
		{
			if (node->key == key)
				break;
			prev = node;
		}
	}

	/*
	 * Remove node from the list to replace it at the beginning.
	 */
	if (node && prev)
	{
		prev->next = node->next;
		node->next = NULL;
	}

	return node;
}

/*
 * @brief Increase the size of the hash table by approx.  2 * current size
 * @param hash: the hash table to increase the size of
 * @return Returns TRUE on success, FALSE on error
 */
static int _eina_hash_increase(Eina_Hash *hash)
{
	void *old;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	/* Max size reached so return FALSE */
	if ((eina_prime_table[hash->size] == PRIME_MAX) || (hash->size
			== PRIME_TABLE_MAX))
		return FALSE;

	/*
	 * Increase the size of the hash and save a pointer to the old data
	 */
	hash->size++;
	old = hash->buckets;

	/*
	 * Allocate a new bucket area, of the new larger size
	 */
	hash->buckets = calloc(eina_prime_table[hash->size],
			sizeof(Eina_Hash_Node *));

	/*
	 * Make sure the allocation succeeded, if not replace the old data and
	 * return a failure.
	 */
	if (!hash->buckets)
	{
		hash->buckets = old;
		hash->size--;
		return FALSE;
	}
	hash->nodes = 0;

	/*
	 * Now move all of the old data into the new bucket area
	 */
	if (_eina_hash_rehash(hash, old, hash->size - 1))
	{
		FREE(old);
		return TRUE;
	}

	/*
	 * Free the old buckets regardless of success.
	 */
	FREE(old);

	return FALSE;
}

/*
 * @brief Decrease the size of the hash table by < 1/2 * current size
 * @param hash: the hash table to decrease the size of
 * @return Returns TRUE on success, FALSE on error
 */
static int _eina_hash_decrease(Eina_Hash *hash)
{
	Eina_Hash_Node **old;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);

	if (eina_prime_table[hash->size] == PRIME_MIN)
		return FALSE;

	/*
	 * Decrease the hash size and store a pointer to the old data
	 */
	hash->size--;
	old = hash->buckets;

	/*
	 * Allocate a new area to store the data
	 */
	hash->buckets = (Eina_Hash_Node **)calloc(eina_prime_table[hash->size],
			sizeof(Eina_Hash_Node *));

	/*
	 * Make sure allocation succeeded otherwise rreturn to the previous
	 * state
	 */
	if (!hash->buckets)
	{
		hash->buckets = old;
		hash->size++;
		return FALSE;
	}

	hash->nodes = 0;

	if (_eina_hash_rehash(hash, old, hash->size + 1))
	{
		FREE(old);
		return TRUE;
	}

	return FALSE;
}

/*
 * @brief Rehash the nodes of a table into the hash table
 * @param hash: the hash to place the nodes of the table
 * @param table: the table to remove the nodes from and place in hash
 * @return Returns TRUE on success, FALSE on error
 */
inline int _eina_hash_rehash(Eina_Hash *hash, Eina_Hash_Node **old_table,
		int old_size)
{
	unsigned int i;
	Eina_Hash_Node *old;

	CHECK_PARAM_POINTER_RETURN("hash", hash, FALSE);
	CHECK_PARAM_POINTER_RETURN("old_table", old_table, FALSE);

	for (i = 0; i < eina_prime_table[old_size]; i++)
	{
		/* Hash into a new list to avoid loops of rehashing the same nodes */
		while ((old = old_table[i]))
		{
			old_table[i] = old->next;
			old->next = NULL;
			_eina_hash_add_node(hash, old);
		}
	}

	return TRUE;
}

/*
 * @brief Create a new hash node for key and value storage
 * @param key: the key for this node
 * @param value: the value that the key references
 * @return Returns NULL on error, a new hash node on success
 */
static Eina_Hash_Node * _eina_hash_node_new(void *key, void *value)
{
	Eina_Hash_Node *node;

	node = (Eina_Hash_Node *)malloc(sizeof(Eina_Hash_Node));
	if (!node)
		return NULL;

	if (!_eina_hash_node_init(node, key, value))
	{
		FREE(node);
		return NULL;
	}

	return node;
}

/*
 * @brief Initialize a hash node to some sane default values
 * @param node: the node to set the values
 * @param key: the key to reference this node
 * @param value: the value that key refers to
 * @return Returns TRUE on success, FALSE on error
 */
static int _eina_hash_node_init(Eina_Hash_Node *node, void *key, void *value)
{
	CHECK_PARAM_POINTER_RETURN("node", node, FALSE);

	node->key = key;
	node->value = value;

	return TRUE;
}

/*
 * @brief Destroy a node and call the specified callbacks to free data
 * @param node: the node to be destroyed
 * @param keyd: the function to free the key
 * @param valued: the function  to free the value
 * @return Returns TRUE on success, FALSE on error
 */
static int _eina_hash_node_destroy(Eina_Hash_Node *node, Eina_Free_Cb keyd,
		Eina_Free_Cb valued)
{
	CHECK_PARAM_POINTER_RETURN("node", node, FALSE);

	if (keyd)
		keyd(node->key);

	if (valued)
		valued(node->value);

	FREE(node);

	return TRUE;
}

/**
 * Just casts the key to an unsigned int
 * @param  key The key to return compute a hash value
 * @return The key cast to an unsigned int.
 */
EAPI unsigned int eina_direct_hash(const void *key)
{
	return ((unsigned int) key);
}
/**
 * Compute the hash value of a string
 * @param  key A pointer to the string to compute a hash value
 * @return A computed hash value for @a key.
 */
EAPI unsigned int eina_str_hash(const void *key)
{
	int i;
	unsigned int mask;
	unsigned int value = 0;
	const char *k = key;

	if (!k)
		return 0;

	mask = (sizeof(unsigned int) * 8) - 1;

	for (i = 0; k[i] != '\0'; i++)
	{
		value ^= ((unsigned int) k[i] << ((i * 5) & mask));
	}

	return value;
}

/**
 * Perform a direct comparison of two keys' values
 * @param  key1 The first key to compare
 * @param  key2 The second key to compare
 * @return A strcmp style value to indicate the larger key
 */
EAPI int eina_direct_compare(const void *key1, const void *key2)
{
	unsigned int k1, k2;

	k1 = (unsigned int) key1;
	k2 = (unsigned int) key2;

	if (k1 > k2)
		return 1;

	if (k1 < k2)
		return -1;

	return 0;
}

/**
 * Perform a string comparison of two keys values
 * @param  key1 The first key to compare
 * @param  key2 The second key to compare
 * @return A strcmp style value to indicate the larger key
 */
EAPI int eina_str_compare(const void *key1, const void *key2)
{
	const char *k1, *k2;

	if (!key1 || !key2)
		return eina_direct_compare(key1, key2);
	else if (key1 == key2)
		return 0;

	k1 = key1;
	k2 = key2;

	return strcmp(k1, k2);
}

