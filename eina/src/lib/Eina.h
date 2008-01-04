#ifndef _EDATA_H
#define _EDATA_H

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/* we need this for size_t */
#include <stddef.h>

/**
 * @mainpage Eina
 * @file Eina.h
 * @brief Contains threading, list, hash, debugging and tree functions.
 */

# ifdef __cplusplus
extern "C" {
# endif

# ifdef __sgi
#  define __FUNCTION__ "unknown"
#  ifndef __cplusplus
#   define inline
#  endif
# endif
  
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

EAPI extern const unsigned int eina_prime_table[];

# define EDATA_SORT_MIN 0
# define EDATA_SORT_MAX 1

typedef void (*Eina_For_Each) (void *value, void *user_data);
# define EDATA_FOR_EACH(function) ((Eina_For_Each)function)

typedef void (*Eina_Free_Cb) (void *data);
# define EDATA_FREE_CB(func) ((Eina_Free_Cb)func)

typedef unsigned int (*Eina_Hash_Cb) (const void *key);
# define EDATA_HASH_CB(function) ((Eina_Hash_Cb)function)

typedef int (*Eina_Compare_Cb) (const void *data1, const void *data2);
# define EDATA_COMPARE_CB(function) ((Eina_Compare_Cb)function)

/**
 * @defgroup List_Group List
 * @{
 */
typedef struct _eina_list Eina_List;
# define EDATA_LIST(list) ((Eina_List *)list)

typedef struct _eina_list_node Eina_List_Node;
# define EDATA_LIST_NODE(node) ((Eina_List_Node *)node)

typedef struct _eina_strbuf Eina_Strbuf;
# define EDATA_STRBUF(buf) ((Eina_Strbuf *)buf)

struct _eina_list_node {
void *data;
struct _eina_list_node *next;
};

struct _eina_list {
	Eina_List_Node *first;	/* The first node in the list */
	Eina_List_Node *last;	/* The last node in the list */
	Eina_List_Node *current;	/* The current node in the list */

	Eina_Free_Cb free_func;  /* The callback to free data in nodes */

	int nodes;		/* The number of nodes in the list */
	int index;		/* The position from the front of the
				 list of current node */
};


/* Creating and initializing new list structures */
EAPI Eina_List *eina_list_new(void);
EAPI int eina_list_init(Eina_List *list);

/* Adding items to the list */
EAPI int eina_list_append(Eina_List * list, void *_data);
EAPI int eina_list_prepend(Eina_List * list, void *_data);
EAPI int eina_list_insert(Eina_List * list, void *_data);
EAPI int eina_list_append_list(Eina_List * list, Eina_List * append);
EAPI int eina_list_prepend_list(Eina_List * list, Eina_List * prepend);

/* Removing items from the list */
EAPI int eina_list_remove_destroy(Eina_List *list);
EAPI void *eina_list_remove(Eina_List * list);
EAPI void *eina_list_first_remove(Eina_List * list);
EAPI void *eina_list_last_remove(Eina_List * list);

/* Retrieve the current position in the list */
EAPI void *eina_list_current(Eina_List * list);
EAPI void *eina_list_first(Eina_List * list);
EAPI void *eina_list_last(Eina_List * list);
EAPI int eina_list_index(Eina_List * list);
EAPI int eina_list_count(Eina_List * list);

/** 
 * @defgroup Eina_List_Traverse_Group Traversal Functions
 * @{
 */
EAPI int eina_list_for_each(Eina_List *list, Eina_For_Each function,
			void *user_data);
EAPI void *eina_list_first_goto(Eina_List * list);
EAPI void *eina_list_last_goto(Eina_List * list);
EAPI void *eina_list_index_goto(Eina_List * list, int index);
EAPI void *eina_list_goto(Eina_List * list, const void *_data);
/** @} */

/* Traversing the list and returning data */
EAPI void *eina_list_next(Eina_List * list);
EAPI void *eina_list_find(Eina_List *list, Eina_Compare_Cb function,
const void *user_data);

/* Sorting the list */
EAPI int eina_list_sort(Eina_List *list, Eina_Compare_Cb compare,
			  char order);
EAPI int eina_list_mergesort(Eina_List *list, Eina_Compare_Cb compare,
			  char order);
EAPI int eina_list_heapsort(Eina_List *list, Eina_Compare_Cb compare,
			  char order);

/* Check to see if there is any data in the list */
EAPI int eina_list_empty_is(Eina_List * list);

/* Remove every node in the list without freeing the list itself */
EAPI int eina_list_clear(Eina_List * list);
/* Free the list and it's contents */
EAPI void eina_list_destroy(Eina_List *list);

/* Creating and initializing list nodes */
EAPI Eina_List_Node *eina_list_node_new(void);
EAPI int eina_list_node_init(Eina_List_Node *newNode);

/* Destroying nodes */
EAPI int eina_list_node_destroy(Eina_List_Node * _e_node, Eina_Free_Cb free_func);

EAPI int eina_list_free_cb_set(Eina_List * list, Eina_Free_Cb free_func);
/** @} */


/**
 * @defgroup DList_Group Double Linked List
 * @{
 */

typedef Eina_List Eina_DList;
# define EDATA_DLIST(dlist) ((Eina_DList *)dlist)

typedef struct _eina_dlist_node Eina_DList_Node;
# define EDATA_DLIST_NODE(dlist) ((Eina_DList_Node *)dlist)

struct _eina_dlist_node {
Eina_List_Node single;
Eina_DList_Node *previous;
};

/* Creating and initializing new list structures */
EAPI Eina_DList *eina_dlist_new(void);
EAPI int eina_dlist_init(Eina_DList *list);
EAPI void eina_dlist_destroy(Eina_DList *list);

/* Adding items to the list */
EAPI int eina_dlist_append(Eina_DList * _e_dlist, void *_data);
EAPI int eina_dlist_prepend(Eina_DList * _e_dlist, void *_data);
EAPI int eina_dlist_insert(Eina_DList * _e_dlist, void *_data);
EAPI int eina_dlist_append_list(Eina_DList * _e_dlist, Eina_DList * append);
EAPI int eina_dlist_prepend_list(Eina_DList * _e_dlist, Eina_DList * prepend);

/* Info about list's state */
EAPI void *eina_dlist_current(Eina_DList *list);
EAPI int eina_dlist_index(Eina_DList *list);
# define eina_dlist_count(list) eina_list_count(EDATA_LIST(list))

/* Removing items from the list */
EAPI void *eina_dlist_remove(Eina_DList * _e_dlist);
EAPI void *eina_dlist_first_remove(Eina_DList * _e_dlist);
EAPI int eina_dlist_remove_destroy(Eina_DList *list);
EAPI void *eina_dlist_last_remove(Eina_DList * _e_dlist);

/* Traversing the list */
# define eina_dlist_for_each(list, function, user_data) \
eina_list_for_each(EDATA_LIST(list), function, user_data)
EAPI void *eina_dlist_first_goto(Eina_DList * _e_dlist);
EAPI void *eina_dlist_last_goto(Eina_DList * _e_dlist);
EAPI void *eina_dlist_index_goto(Eina_DList * _e_dlist, int index);
EAPI void *eina_dlist_goto(Eina_DList * _e_dlist, void *_data);

/* Traversing the list and returning data */
EAPI void *eina_dlist_next(Eina_DList * list);
EAPI void *eina_dlist_previous(Eina_DList * list);

/* Sorting the list */
EAPI int eina_dlist_sort(Eina_DList *list, Eina_Compare_Cb compare,
			  char order);
EAPI int eina_dlist_mergesort(Eina_DList *list, Eina_Compare_Cb compare,
			  char order);
# define eina_dlist_heapsort(list, compare, order) \
eina_list_heapsort(list, compare, order)

/* Check to see if there is any data in the list */
EAPI int eina_dlist_empty_is(Eina_DList * _e_dlist);

/* Remove every node in the list without free'ing it */
EAPI int eina_dlist_clear(Eina_DList * _e_dlist);

/* Creating and initializing list nodes */
EAPI int eina_dlist_node_init(Eina_DList_Node * node);
EAPI Eina_DList_Node *eina_dlist_node_new(void);

/* Destroying nodes */
EAPI int eina_dlist_node_destroy(Eina_DList_Node * node, Eina_Free_Cb free_func);

EAPI int eina_dlist_free_cb_set(Eina_DList * dlist, Eina_Free_Cb free_func);

/** @} */

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
# define EDATA_HASH_NODE(hash) ((Eina_Hash_Node *)hash)

struct _eina_hash_node {
	Eina_Hash_Node *next; /* Pointer to the next node in the bucket list */
	void *key;	     /* The key for the data node */
	void *value;	     /* The value associated with this node */
};

typedef struct _eina_hash Eina_Hash;
# define EDATA_HASH(hash) ((Eina_Hash *)hash)

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

typedef struct _eina_heap Eina_Sheap;
# define EDATA_HEAP(heap) ((Eina_Sheap *)heap)

struct _eina_heap {
void **data;
int size;
int space;

char order, sorted;

/* Callback for comparing node values, default is direct comparison */
Eina_Compare_Cb compare;

/* Callback for freeing node data, default is NULL */
Eina_Free_Cb free_func;
};

EAPI Eina_Sheap *eina_sheap_new(Eina_Compare_Cb compare, int size);
EAPI void eina_sheap_destroy(Eina_Sheap *heap);
EAPI int eina_sheap_init(Eina_Sheap *heap, Eina_Compare_Cb compare, int size);
EAPI int eina_sheap_free_cb_set(Eina_Sheap *heap, Eina_Free_Cb free_func);
EAPI int eina_sheap_insert(Eina_Sheap *heap, void *data);
EAPI void *eina_sheap_extract(Eina_Sheap *heap);
EAPI void *eina_sheap_extreme(Eina_Sheap *heap);
EAPI int eina_sheap_change(Eina_Sheap *heap, void *item, void *newval);
EAPI int eina_sheap_compare_set(Eina_Sheap *heap, Eina_Compare_Cb compare);
EAPI void eina_sheap_order_set(Eina_Sheap *heap, char order);
EAPI void eina_sheap_sort(Eina_Sheap *heap);

EAPI void *eina_sheap_item(Eina_Sheap *heap, int i);


typedef struct _eina_string Eina_String;
struct _eina_string {
char *string;
int references;
};

EAPI int eina_string_init(void);
EAPI void eina_string_shutdown(void);
EAPI const char *eina_string_instance(const char *string);
EAPI void eina_string_release(const char *string);
EAPI void eina_string_hash_dump_graph(void);
EAPI void eina_string_hash_dump_stats(void);


typedef struct _Eina_Tree_Node Eina_Tree_Node;
# define EDATA_TREE_NODE(object) ((Eina_Tree_Node *)object)
struct _Eina_Tree_Node {

/* The actual data for each node */
void *key;
void *value;

/* Pointers to surrounding nodes */
Eina_Tree_Node *parent;
Eina_Tree_Node *left_child;
Eina_Tree_Node *right_child;

/* Book keeping information for quicker balancing of the tree */
int max_right;
int max_left;
};

typedef struct _Eina_Tree Eina_Tree;
# define EDATA_TREE(object) ((Eina_Tree *)object)
struct _Eina_Tree {
/* Nodes of the tree */
Eina_Tree_Node *tree;

/* Callback for comparing node values, default is direct comparison */
Eina_Compare_Cb compare_func;

/* Callback for freeing node data, default is NULL */
Eina_Free_Cb free_value;
/* Callback for freeing node key, default is NULL */
Eina_Free_Cb free_key;
};

/* Some basic tree functions */
/* Allocate and initialize a new tree */
EAPI Eina_Tree *eina_tree_new(Eina_Compare_Cb compare_func);
/* Initialize a new tree */
EAPI int eina_tree_init(Eina_Tree * tree, Eina_Compare_Cb compare_func);

/* Free the tree */
EAPI int eina_tree_destroy(Eina_Tree * tree);
/* Check to see if the tree has any nodes in it */
EAPI int eina_tree_empty_is(Eina_Tree * tree);

/* Retrieve the value associated with key */
EAPI void *eina_tree_get(Eina_Tree * tree, const void *key);
EAPI Eina_Tree_Node *eina_tree_get_node(Eina_Tree * tree, const void *key);
/* Retrieve the value of node with key greater than or equal to key */
EAPI void *eina_tree_closest_larger_get(Eina_Tree * tree, const void *key);
/* Retrieve the value of node with key less than or equal to key */
EAPI void *eina_tree_closest_smaller_get(Eina_Tree * tree, const void *key);

/* Set the value associated with key to value */
EAPI int eina_tree_set(Eina_Tree * tree, void *key, void *value);
/* Remove the key from the tree */
EAPI int eina_tree_remove(Eina_Tree * tree, const void *key);

/* Add a node to the tree */
EAPI int eina_tree_node_add(Eina_Tree * tree, Eina_Tree_Node * node);
/* Remove a node from the tree */
EAPI int eina_tree_node_remove(Eina_Tree * tree, Eina_Tree_Node * node);

/* For each node in the tree perform the for_each_func function */
/* For this one pass in the node */
EAPI int eina_tree_for_each_node(Eina_Tree * tree, Eina_For_Each for_each_func,
			     void *user_data);
/* And here pass in the node's value */
EAPI int eina_tree_for_each_node_value(Eina_Tree * tree,
				   Eina_For_Each for_each_func,
				   void *user_data);

/* Some basic node functions */
/* Initialize a node */
EAPI int eina_tree_node_init(Eina_Tree_Node * new_node);
/* Allocate and initialize a new node */
EAPI Eina_Tree_Node *eina_tree_node_new(void);
/* Free the desired node */
EAPI int eina_tree_node_destroy(Eina_Tree_Node * node, 
	   Eina_Free_Cb free_value, Eina_Free_Cb free_key);

/* Set the node's key to key */
EAPI int eina_tree_node_key_set(Eina_Tree_Node * node, void *key);
/* Retrieve the key in node */
EAPI void *eina_tree_node_key_get(Eina_Tree_Node * node);

/* Set the node's value to value */
EAPI int eina_tree_node_value_set(Eina_Tree_Node * node, void *value);
/* Retrieve the value in node */
EAPI void *eina_tree_node_value_get(Eina_Tree_Node * node);

/* Add a function to free the data stored in nodes */
EAPI int eina_tree_free_value_cb_set(Eina_Tree * tree, Eina_Free_Cb free_value);
/* Add a function to free the keys stored in nodes */
EAPI int eina_tree_free_key_cb_set(Eina_Tree * tree, Eina_Free_Cb free_key);


EAPI Eina_Strbuf * eina_strbuf_new(void);
EAPI void eina_strbuf_free(Eina_Strbuf *buf);
EAPI void eina_strbuf_append(Eina_Strbuf *buf, const char *str);
EAPI void eina_strbuf_append_char(Eina_Strbuf *buf, char c);
EAPI void eina_strbuf_insert(Eina_Strbuf *buf, const char *str, 
			 size_t pos);
# define eina_strbuf_prepend(buf, str) eina_strbuf_insert(buf, str, 0)
EAPI const char * eina_strbuf_string_get(Eina_Strbuf *buf);
EAPI size_t eina_strbuf_length_get(Eina_Strbuf *buf);
EAPI int eina_strbuf_replace(Eina_Strbuf *buf, const char *str, 
			 const char *with, unsigned int n);
# define eina_strbuf_replace_first(buf, str, with) \
eina_strbuf_replace(buf, str, with, 1)
EAPI int eina_strbuf_replace_all(Eina_Strbuf *buf, const char *str,
			     const char *with);

EAPI int eina_direct_compare(const void *key1, const void *key2);
EAPI int eina_str_compare(const void *key1, const void *key2);

EAPI unsigned int eina_direct_hash(const void *key);
EAPI unsigned int eina_str_hash(const void *key);

/**
 * @defgroup Array_Group Array
 * @{
 */
typedef void (*Eina_Array_Alloc) (void *user_data, int num);
#define EDATA_ARRAY_ALLOC(function) ((Eina_Array_Alloc)function)
typedef void (*Eina_Array_Free) (void *user_data);
#define EDATA_ARRAY_FREE(function) ((Eina_Array_Free)function)

typedef struct _Eina_Array Eina_Array;
EAPI void eina_array_free(Eina_Array *a);
EAPI Eina_Array *eina_array_new(void *data, Eina_Array_Alloc alloc_cb, Eina_Array_Free free_cb);
EAPI void eina_array_element_new(Eina_Array *a);

/** @} */

#ifdef __cplusplus
}
#endif
#endif				/* _EDATA_DATA_H */
