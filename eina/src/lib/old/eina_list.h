#ifndef EINA_LIST_H_
#define EINA_LIST_H_

/**
 * @defgroup List_Group List
 * @{
 */
typedef struct _eina_list Eina_List;
#define EINA_LIST(list) ((Eina_List *)list)

typedef struct _eina_list_node Eina_List_Node;
#define EINA_LIST_NODE(node) ((Eina_List_Node *)node)

typedef struct _eina_strbuf Eina_Strbuf;
#define EINA_STRBUF(buf) ((Eina_Strbuf *)buf)

struct _eina_list_node {
	void *data;
	struct _eina_list_node *next;
};

struct _eina_list {
	Eina_List_Node *first; /* The first node in the list */
	Eina_List_Node *last; /* The last node in the list */
	Eina_List_Node *current; /* The current node in the list */

	Eina_Free_Cb free_func; /* The callback to free data in nodes */

	int nodes; /* The number of nodes in the list */
	int index; /* The position from the front of the
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
EAPI int eina_list_sort(Eina_List *list, Eina_Compare_Cb compare, char order);
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
EAPI int eina_list_node_destroy(Eina_List_Node * _e_node,
		Eina_Free_Cb free_func);

EAPI int eina_list_free_cb_set(Eina_List * list, Eina_Free_Cb free_func);
/** @} */

/**
 * @defgroup DList_Group Double Linked List
 * @{
 */

typedef Eina_List Eina_DList;
#define EINA_DLIST(dlist) ((Eina_DList *)dlist)

typedef struct _eina_dlist_node Eina_DList_Node;
#define EINA_DLIST_NODE(dlist) ((Eina_DList_Node *)dlist)

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
#define eina_dlist_count(list) eina_list_count(EINA_LIST(list))

/* Removing items from the list */
EAPI void *eina_dlist_remove(Eina_DList * _e_dlist);
EAPI void *eina_dlist_first_remove(Eina_DList * _e_dlist);
EAPI int eina_dlist_remove_destroy(Eina_DList *list);
EAPI void *eina_dlist_last_remove(Eina_DList * _e_dlist);

/* Traversing the list */
#define eina_dlist_for_each(list, function, user_data) \
eina_list_for_each(EINA_LIST(list), function, user_data)
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
#define eina_dlist_heapsort(list, compare, order) \
eina_list_heapsort(list, compare, order)

/* Check to see if there is any data in the list */
EAPI int eina_dlist_empty_is(Eina_DList * _e_dlist);

/* Remove every node in the list without free'ing it */
EAPI int eina_dlist_clear(Eina_DList * _e_dlist);

/* Creating and initializing list nodes */
EAPI int eina_dlist_node_init(Eina_DList_Node * node);
EAPI Eina_DList_Node *eina_dlist_node_new(void);

/* Destroying nodes */
EAPI int eina_dlist_node_destroy(Eina_DList_Node * node,
	Eina_Free_Cb free_func);

EAPI int eina_dlist_free_cb_set(Eina_DList * dlist, Eina_Free_Cb free_func);

/** @} */
#endif /* EINA_LIST_H */
