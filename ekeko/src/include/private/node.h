#ifndef NODE_H_
#define NODE_H_

struct _Ekeko_Node
{
	EINA_INLIST;
	Ekeko_Node_Type type;
	const char *name;
	Ekeko_Value value;
	/* used on attributes and elements, to know when some child or attribute
	 * has changed
	 */
	int changed;
	/* only used on attributes */
	struct
	{
		Ekeko_Value prev; 
	} attr;
	Ekeko_Node_List *childs;
	Ekeko_Node_Named_Map *attributes;
	Eina_Hash *user;
#ifdef EKEKO_EVENT
	Eina_Hash *events;
#endif
};

void ekeko_node_initialize(Ekeko_Node *n);
Eina_Bool ekeko_node_changed(Ekeko_Node *n);

#endif /*NODE_H_*/
