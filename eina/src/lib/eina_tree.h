#ifndef EINA_TREE_H_
#define EINA_TREE_H_

typedef struct _Eina_Tree_Node Eina_Tree_Node;
#define EINA_TREE_NODE(object) ((Eina_Tree_Node *)object)
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
#define EINA_TREE(object) ((Eina_Tree *)object)
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

#endif /* EINA_TREE_H_ */
