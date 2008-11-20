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
	Eina_Inlist *childs;
	Ekeko_Node *parent;
	Ekeko_Node_Named_Map *attributes;
	Ekeko_Document *owner;
	Eina_Hash *user;
	Eina_Hash *events;
};

void ekeko_node_initialize(Ekeko_Node *n);
Eina_Bool ekeko_node_changed(Ekeko_Node *n);
Eina_Bool ekeko_node_event_dispatch(Ekeko_Node *n, Ekeko_Event *e);


#endif /*NODE_H_*/
