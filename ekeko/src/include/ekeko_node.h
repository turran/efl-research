#ifndef EKEKO_NODE_H_
#define EKEKO_NODE_H_

typedef struct _Ekeko_Node_Private Ekeko_Node_Private;

struct _Ekeko_Node
{
	Ekeko_Node_Private *p;
};

typedef enum _Ekeko_Node_Type
{
	EKEKO_NODE_ELEMENT = 1,
	EKEKO_NODE_ATTRIBUTE = 2,
	EKEKO_NODE_TEXT = 3,
	EKEKO_NODE_CDATA_SECTION = 4,
	EKEKO_NODE_DOCUMENT = 9
} Ekeko_Node_Type;

EAPI Ekeko_Value * ekeko_node_value_get(Ekeko_Node *n);
EAPI void ekeko_node_value_set(Ekeko_Node *n, const char *v);
EAPI void ekeko_node_value_int_set(Ekeko_Node *n, int v);
EAPI Ekeko_Node_Type ekeko_node_type(Ekeko_Node *);
EAPI Ekeko_Node * ekeko_node_parent(Ekeko_Node *);
/* LEVEL 2 */
EAPI Ekeko_Document * ekeko_node_document_owner(Ekeko_Node *);
/* LEVEL 3 */
EAPI void ekeko_node_user_set(Ekeko_Node *n, const char *key, void *data);
EAPI void * ekeko_node_user_get(Ekeko_Node *n, const char *key);
/* OWN */
EAPI void ekeko_node_process(Ekeko_Node *n);

#endif /*EKEKO_NODE_H_*/
