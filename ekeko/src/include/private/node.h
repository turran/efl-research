#ifndef NODE_H_
#define NODE_H_

void ekeko_node_initialize(Ekeko_Node *n);
Eina_Bool ekeko_node_changed(Ekeko_Node *n);
Eina_Bool ekeko_node_event_dispatch(Ekeko_Node *n, Ekeko_Event *e);


#endif /*NODE_H_*/
