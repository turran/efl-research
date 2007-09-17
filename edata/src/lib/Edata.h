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
 * @file Edata_Data.h
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

   EAPI extern const unsigned int edata_prime_table[];
   
# define EDATA_SORT_MIN 0
# define EDATA_SORT_MAX 1

   typedef void (*Edata_For_Each) (void *value, void *user_data);
# define EDATA_FOR_EACH(function) ((Edata_For_Each)function)
   
   typedef void (*Edata_Free_Cb) (void *data);
# define EDATA_FREE_CB(func) ((Edata_Free_Cb)func)
   
   typedef unsigned int (*Edata_Hash_Cb) (const void *key);
# define EDATA_HASH_CB(function) ((Edata_Hash_Cb)function)
   
   typedef int (*Edata_Compare_Cb) (const void *data1, const void *data2);
# define EDATA_COMPARE_CB(function) ((Edata_Compare_Cb)function)
   
   typedef struct _edata_list Edata_List;
# define EDATA_LIST(list) ((Edata_List *)list)
   
   typedef struct _edata_list_node Edata_List_Node;
# define EDATA_LIST_NODE(node) ((Edata_List_Node *)node)

   typedef struct _edata_strbuf Edata_Strbuf;
# define EDATA_STRBUF(buf) ((Edata_Strbuf *)buf)
   
   struct _edata_list_node {
      void *data;
      struct _edata_list_node *next;
   };
   
   struct _edata_list {
      Edata_List_Node *first;	/* The first node in the list */
      Edata_List_Node *last;	/* The last node in the list */
      Edata_List_Node *current;	/* The current node in the list */
      
      Edata_Free_Cb free_func;  /* The callback to free data in nodes */
      
      int nodes;		/* The number of nodes in the list */
      int index;		/* The position from the front of the
				 list of current node */
   };
   
   EAPI int edata_direct_compare(const void *key1, const void *key2);
   EAPI int edata_str_compare(const void *key1, const void *key2);
   
   EAPI unsigned int edata_direct_hash(const void *key);
   EAPI unsigned int edata_str_hash(const void *key);
   
   /* Creating and initializing new list structures */
   EAPI Edata_List *edata_list_new(void);
   EAPI int edata_list_init(Edata_List *list);
   
   /* Adding items to the list */
   EAPI int edata_list_append(Edata_List * list, void *_data);
   EAPI int edata_list_prepend(Edata_List * list, void *_data);
   EAPI int edata_list_insert(Edata_List * list, void *_data);
   EAPI int edata_list_append_list(Edata_List * list, Edata_List * append);
   EAPI int edata_list_prepend_list(Edata_List * list, Edata_List * prepend);
   
   /* Removing items from the list */
   EAPI int edata_list_remove_destroy(Edata_List *list);
   EAPI void *edata_list_remove(Edata_List * list);
   EAPI void *edata_list_first_remove(Edata_List * list);
   EAPI void *edata_list_last_remove(Edata_List * list);
   
   /* Retrieve the current position in the list */
   EAPI void *edata_list_current(Edata_List * list);
   EAPI void *edata_list_first(Edata_List * list);
   EAPI void *edata_list_last(Edata_List * list);
   EAPI int edata_list_index(Edata_List * list);
   EAPI int edata_list_count(Edata_List * list);
   
   /* Traversing the list */
   EAPI int edata_list_for_each(Edata_List *list, Edata_For_Each function,
				void *user_data);
   EAPI void *edata_list_first_goto(Edata_List * list);
   EAPI void *edata_list_last_goto(Edata_List * list);
   EAPI void *edata_list_index_goto(Edata_List * list, int index);
   EAPI void *edata_list_goto(Edata_List * list, const void *_data);
   
   /* Traversing the list and returning data */
   EAPI void *edata_list_next(Edata_List * list);
   EAPI void *edata_list_find(Edata_List *list, Edata_Compare_Cb function,
        const void *user_data);

   /* Sorting the list */
   EAPI int edata_list_sort(Edata_List *list, Edata_Compare_Cb compare,
                                  char order);
   EAPI int edata_list_mergesort(Edata_List *list, Edata_Compare_Cb compare,
                                  char order);
   EAPI int edata_list_heapsort(Edata_List *list, Edata_Compare_Cb compare,
                                  char order);
   
   /* Check to see if there is any data in the list */
   EAPI int edata_list_empty_is(Edata_List * list);
   
   /* Remove every node in the list without freeing the list itself */
   EAPI int edata_list_clear(Edata_List * list);
   /* Free the list and it's contents */
   EAPI void edata_list_destroy(Edata_List *list);
   
   /* Creating and initializing list nodes */
   EAPI Edata_List_Node *edata_list_node_new(void);
   EAPI int edata_list_node_init(Edata_List_Node *newNode);
   
   /* Destroying nodes */
   EAPI int edata_list_node_destroy(Edata_List_Node * _e_node, Edata_Free_Cb free_func);
   
   EAPI int edata_list_free_cb_set(Edata_List * list, Edata_Free_Cb free_func);
   
   typedef Edata_List Edata_DList;
# define EDATA_DLIST(dlist) ((Edata_DList *)dlist)
   
   typedef struct _edata_dlist_node Edata_DList_Node;
# define EDATA_DLIST_NODE(dlist) ((Edata_DList_Node *)dlist)
   
   struct _edata_dlist_node {
      Edata_List_Node single;
      Edata_DList_Node *previous;
   };
   
   /* Creating and initializing new list structures */
   EAPI Edata_DList *edata_dlist_new(void);
   EAPI int edata_dlist_init(Edata_DList *list);
   EAPI void edata_dlist_destroy(Edata_DList *list);
   
   /* Adding items to the list */
   EAPI int edata_dlist_append(Edata_DList * _e_dlist, void *_data);
   EAPI int edata_dlist_prepend(Edata_DList * _e_dlist, void *_data);
   EAPI int edata_dlist_insert(Edata_DList * _e_dlist, void *_data);
   EAPI int edata_dlist_append_list(Edata_DList * _e_dlist, Edata_DList * append);
   EAPI int edata_dlist_prepend_list(Edata_DList * _e_dlist, Edata_DList * prepend);
   
   /* Info about list's state */
   EAPI void *edata_dlist_current(Edata_DList *list);
   EAPI int edata_dlist_index(Edata_DList *list);
# define edata_dlist_count(list) edata_list_count(EDATA_LIST(list))
   
   /* Removing items from the list */
   EAPI void *edata_dlist_remove(Edata_DList * _e_dlist);
   EAPI void *edata_dlist_first_remove(Edata_DList * _e_dlist);
   EAPI int edata_dlist_remove_destroy(Edata_DList *list);
   EAPI void *edata_dlist_last_remove(Edata_DList * _e_dlist);
   
   /* Traversing the list */
# define edata_dlist_for_each(list, function, user_data) \
   edata_list_for_each(EDATA_LIST(list), function, user_data)
   EAPI void *edata_dlist_first_goto(Edata_DList * _e_dlist);
   EAPI void *edata_dlist_last_goto(Edata_DList * _e_dlist);
   EAPI void *edata_dlist_index_goto(Edata_DList * _e_dlist, int index);
   EAPI void *edata_dlist_goto(Edata_DList * _e_dlist, void *_data);
   
   /* Traversing the list and returning data */
   EAPI void *edata_dlist_next(Edata_DList * list);
   EAPI void *edata_dlist_previous(Edata_DList * list);
   
   /* Sorting the list */
   EAPI int edata_dlist_sort(Edata_DList *list, Edata_Compare_Cb compare,
                                  char order);
   EAPI int edata_dlist_mergesort(Edata_DList *list, Edata_Compare_Cb compare,
                                  char order);
# define edata_dlist_heapsort(list, compare, order) \
   edata_list_heapsort(list, compare, order)
   
   /* Check to see if there is any data in the list */
   EAPI int edata_dlist_empty_is(Edata_DList * _e_dlist);
   
   /* Remove every node in the list without free'ing it */
   EAPI int edata_dlist_clear(Edata_DList * _e_dlist);
   
   /* Creating and initializing list nodes */
   EAPI int edata_dlist_node_init(Edata_DList_Node * node);
   EAPI Edata_DList_Node *edata_dlist_node_new(void);
   
   /* Destroying nodes */
   EAPI int edata_dlist_node_destroy(Edata_DList_Node * node, Edata_Free_Cb free_func);
   
   EAPI int edata_dlist_free_cb_set(Edata_DList * dlist, Edata_Free_Cb free_func);
   
   
   
   /*
    * Hash Table Implementation:
    * 
    * Traditional hash table implementation. I had tried a list of tables
    * approach to save on the realloc's but it ended up being much slower than
    * the traditional approach.
    */
   
   typedef struct _edata_hash_node Edata_Hash_Node;
# define EDATA_HASH_NODE(hash) ((Edata_Hash_Node *)hash)
   
   struct _edata_hash_node {
      Edata_Hash_Node *next; /* Pointer to the next node in the bucket list */
      void *key;	     /* The key for the data node */
      void *value;	     /* The value associated with this node */
   };
   
   typedef struct _edata_hash Edata_Hash;
# define EDATA_HASH(hash) ((Edata_Hash *)hash)
   
   struct _edata_hash {
      Edata_Hash_Node **buckets;
      int size;		/* An index into the table of primes to
			 determine size */
      int nodes;		/* The number of nodes currently in the hash */

      int index;    /* The current index into the bucket table */
      
      Edata_Compare_Cb compare;	/* The function used to compare node values */
      Edata_Hash_Cb hash_func;	/* The callback function to determine hash */
      
      Edata_Free_Cb free_key;	/* The callback function to free key */
      Edata_Free_Cb free_value;	/* The callback function to free value */
   };
   
   /* Create and initialize a hash */
   EAPI Edata_Hash *edata_hash_new(Edata_Hash_Cb hash_func, Edata_Compare_Cb compare);
   EAPI int edata_hash_init(Edata_Hash *hash, Edata_Hash_Cb hash_func, Edata_Compare_Cb compare);
   
   /* Functions related to freeing the data in the hash table */
   EAPI int edata_hash_free_key_cb_set(Edata_Hash *hash, Edata_Free_Cb function);
   EAPI int edata_hash_free_value_cb_set(Edata_Hash *hash, Edata_Free_Cb function);
   EAPI void edata_hash_destroy(Edata_Hash *hash);

   EAPI int edata_hash_count(Edata_Hash *hash);
   EAPI int edata_hash_for_each_node(Edata_Hash *hash, Edata_For_Each for_each_func,
				     void *user_data);
   EAPI Edata_List *edata_hash_keys(Edata_Hash *hash);
   
   /* Retrieve and store data into the hash */
   EAPI void *edata_hash_get(Edata_Hash *hash, const void *key);
   EAPI int edata_hash_set(Edata_Hash *hash, void *key, void *value);
   EAPI int edata_hash_hash_set(Edata_Hash *hash, Edata_Hash *set);
   EAPI void *edata_hash_remove(Edata_Hash *hash, const void *key);
   EAPI void *edata_hash_find(Edata_Hash *hash, Edata_Compare_Cb compare, const void *value);
   EAPI void edata_hash_dump_graph(Edata_Hash *hash);
   EAPI void edata_hash_dump_stats(Edata_Hash *hash);


   typedef struct _edata_path_group Edata_Path_Group;
   struct _edata_path_group
     {
	int id;
	char *name;
	Edata_List *paths;
     };
   
   /*
    * Create a new path group
    */
   EAPI int edata_path_group_new(const char *group_name);
   
   /*
    * Destroy a previous path group
    */
   EAPI void edata_path_group_del(int group_id);
   
   /*
    * Add a directory to be searched for files
    */
   EAPI void edata_path_group_add(int group_id, const char *path);
   
   /*
    * Remove a directory to be searched for files
    */
   EAPI void edata_path_group_remove(int group_id, const char *path);
   
   /*
    * Find the absolute path if it exists in the group of paths
    */
   EAPI char * edata_path_group_find(int group_id, const char *name);
   
   /*
    * Get a list of all the available files in a path set
    */
   EAPI Edata_List * edata_path_group_available(int group_id);
   
   
   typedef struct _edata_plugin Edata_Plugin;
   struct _edata_plugin
     {
	int group;
	char *name;
	void *handle;
     };
   
   /*
    * Load the specified plugin
    */
   EAPI Edata_Plugin *edata_plugin_load(int group_id, const char *plugin);
   
   /*
    * Unload the specified plugin
    */
   EAPI void edata_plugin_unload(Edata_Plugin * plugin);
   
   /*
    * Lookup the specified symbol for the plugin
    */
   EAPI void *edata_plugin_call(Edata_Plugin * plugin, const char *symbol_name);
   
   EAPI Edata_List *edata_plugin_available_get(int group_id);


   typedef struct _edata_heap Edata_Sheap;
# define EDATA_HEAP(heap) ((Edata_Sheap *)heap)
   
   struct _edata_heap {
      void **data;
      int size;
      int space;
      
      char order, sorted;
      
      /* Callback for comparing node values, default is direct comparison */
      Edata_Compare_Cb compare;

      /* Callback for freeing node data, default is NULL */
      Edata_Free_Cb free_func;
   };
   
   EAPI Edata_Sheap *edata_sheap_new(Edata_Compare_Cb compare, int size);
   EAPI void edata_sheap_destroy(Edata_Sheap *heap);
   EAPI int edata_sheap_init(Edata_Sheap *heap, Edata_Compare_Cb compare, int size);
   EAPI int edata_sheap_free_cb_set(Edata_Sheap *heap, Edata_Free_Cb free_func);
   EAPI int edata_sheap_insert(Edata_Sheap *heap, void *data);
   EAPI void *edata_sheap_extract(Edata_Sheap *heap);
   EAPI void *edata_sheap_extreme(Edata_Sheap *heap);
   EAPI int edata_sheap_change(Edata_Sheap *heap, void *item, void *newval);
   EAPI int edata_sheap_compare_set(Edata_Sheap *heap, Edata_Compare_Cb compare);
   EAPI void edata_sheap_order_set(Edata_Sheap *heap, char order);
   EAPI void edata_sheap_sort(Edata_Sheap *heap);
   
   EAPI void *edata_sheap_item(Edata_Sheap *heap, int i);
   
   
   typedef struct _edata_string Edata_String;
   struct _edata_string {
      char *string;
      int references;
   };
   
   EAPI int edata_string_init(void);
   EAPI void edata_string_shutdown(void);
   EAPI const char *edata_string_instance(const char *string);
   EAPI void edata_string_release(const char *string);
   EAPI void edata_string_hash_dump_graph(void);
   EAPI void edata_string_hash_dump_stats(void);
   
   
   typedef struct _Edata_Tree_Node Edata_Tree_Node;
# define EDATA_TREE_NODE(object) ((Edata_Tree_Node *)object)
   struct _Edata_Tree_Node {
      
      /* The actual data for each node */
      void *key;
      void *value;
      
      /* Pointers to surrounding nodes */
      Edata_Tree_Node *parent;
      Edata_Tree_Node *left_child;
      Edata_Tree_Node *right_child;
      
      /* Book keeping information for quicker balancing of the tree */
      int max_right;
      int max_left;
   };
   
   typedef struct _Edata_Tree Edata_Tree;
# define EDATA_TREE(object) ((Edata_Tree *)object)
   struct _Edata_Tree {
      /* Nodes of the tree */
      Edata_Tree_Node *tree;
      
      /* Callback for comparing node values, default is direct comparison */
      Edata_Compare_Cb compare_func;
      
      /* Callback for freeing node data, default is NULL */
      Edata_Free_Cb free_value;
      /* Callback for freeing node key, default is NULL */
      Edata_Free_Cb free_key;
   };
   
   /* Some basic tree functions */
   /* Allocate and initialize a new tree */
   EAPI Edata_Tree *edata_tree_new(Edata_Compare_Cb compare_func);
   /* Initialize a new tree */
   EAPI int edata_tree_init(Edata_Tree * tree, Edata_Compare_Cb compare_func);
   
   /* Free the tree */
   EAPI int edata_tree_destroy(Edata_Tree * tree);
   /* Check to see if the tree has any nodes in it */
   EAPI int edata_tree_empty_is(Edata_Tree * tree);
   
   /* Retrieve the value associated with key */
   EAPI void *edata_tree_get(Edata_Tree * tree, const void *key);
   EAPI Edata_Tree_Node *edata_tree_get_node(Edata_Tree * tree, const void *key);
   /* Retrieve the value of node with key greater than or equal to key */
   EAPI void *edata_tree_closest_larger_get(Edata_Tree * tree, const void *key);
   /* Retrieve the value of node with key less than or equal to key */
   EAPI void *edata_tree_closest_smaller_get(Edata_Tree * tree, const void *key);
   
   /* Set the value associated with key to value */
   EAPI int edata_tree_set(Edata_Tree * tree, void *key, void *value);
   /* Remove the key from the tree */
   EAPI int edata_tree_remove(Edata_Tree * tree, const void *key);
   
   /* Add a node to the tree */
   EAPI int edata_tree_node_add(Edata_Tree * tree, Edata_Tree_Node * node);
   /* Remove a node from the tree */
   EAPI int edata_tree_node_remove(Edata_Tree * tree, Edata_Tree_Node * node);
   
   /* For each node in the tree perform the for_each_func function */
   /* For this one pass in the node */
   EAPI int edata_tree_for_each_node(Edata_Tree * tree, Edata_For_Each for_each_func,
				     void *user_data);
   /* And here pass in the node's value */
   EAPI int edata_tree_for_each_node_value(Edata_Tree * tree,
					   Edata_For_Each for_each_func,
					   void *user_data);
   
   /* Some basic node functions */
   /* Initialize a node */
   EAPI int edata_tree_node_init(Edata_Tree_Node * new_node);
   /* Allocate and initialize a new node */
   EAPI Edata_Tree_Node *edata_tree_node_new(void);
   /* Free the desired node */
   EAPI int edata_tree_node_destroy(Edata_Tree_Node * node, 
		   Edata_Free_Cb free_value, Edata_Free_Cb free_key);
   
   /* Set the node's key to key */
   EAPI int edata_tree_node_key_set(Edata_Tree_Node * node, void *key);
   /* Retrieve the key in node */
   EAPI void *edata_tree_node_key_get(Edata_Tree_Node * node);
   
   /* Set the node's value to value */
   EAPI int edata_tree_node_value_set(Edata_Tree_Node * node, void *value);
   /* Retrieve the value in node */
   EAPI void *edata_tree_node_value_get(Edata_Tree_Node * node);
   
   /* Add a function to free the data stored in nodes */
   EAPI int edata_tree_free_value_cb_set(Edata_Tree * tree, Edata_Free_Cb free_value);
   /* Add a function to free the keys stored in nodes */
   EAPI int edata_tree_free_key_cb_set(Edata_Tree * tree, Edata_Free_Cb free_key);


   EAPI Edata_Strbuf * edata_strbuf_new(void);
   EAPI void edata_strbuf_free(Edata_Strbuf *buf);
   EAPI void edata_strbuf_append(Edata_Strbuf *buf, const char *str);
   EAPI void edata_strbuf_append_char(Edata_Strbuf *buf, char c);
   EAPI void edata_strbuf_insert(Edata_Strbuf *buf, const char *str, 
                                 size_t pos);
# define edata_strbuf_prepend(buf, str) edata_strbuf_insert(buf, str, 0)
   EAPI const char * edata_strbuf_string_get(Edata_Strbuf *buf);
   EAPI size_t edata_strbuf_length_get(Edata_Strbuf *buf);
   EAPI int edata_strbuf_replace(Edata_Strbuf *buf, const char *str, 
                                 const char *with, unsigned int n);
# define edata_strbuf_replace_first(buf, str, with) \
	edata_strbuf_replace(buf, str, with, 1)
   EAPI int edata_strbuf_replace_all(Edata_Strbuf *buf, const char *str,
                                     const char *with);

#ifdef __cplusplus
}
#endif
#endif				/* _EDATA_DATA_H */
