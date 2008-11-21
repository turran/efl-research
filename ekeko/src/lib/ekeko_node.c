#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Ekeko_Event_Listener_Container
{
	Ekeko_Event_Listener listener;
	Eina_Bool capture;
} Ekeko_Event_Listener_Container;

static void _node_unchange_rec(Ekeko_Node *n, int num)
{
	n->changed -= num;
	/* notify backwards */
	if (n->parent)
		_node_unchange_rec(n->parent, num);
}

static void _node_change_rec(Ekeko_Node *n, int num)
{
	n->changed += num;
	/* notify backwards */
	if (n->parent)
		_node_change_rec(n->parent, num);
}

void _attribute_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	int changed = EINA_FALSE;
	Ekeko_Value old;

	old = n->value;
	switch (v->type)
	{
	/* First basic types */
	case EKEKO_ATTRIBUTE_NONE:
		printf("attribute %s hasnt been initialized\n", n->name);
		exit(1);
	case EKEKO_ATTRIBUTE_INT:
		if (v->v.i != n->attr.prev.v.i)
		{
			changed = EINA_TRUE;
			n->value.v.i = v->v.i;
		}
		break;
	case EKEKO_ATTRIBUTE_RECTANGLE:
		{
			Eina_Rectangle *r = &(v->v.r);
			Eina_Rectangle *p = &(n->attr.prev.v.r);
			Eina_Rectangle *c = &(n->value.v.r);
			
			if ((r->x != p->x) || (r->y != p->y) ||
					(r->w != p->w) || (r->h != p->h))
			{
				c->x = r->x;
				c->y = r->y;
				c->w = r->w;
				c->h = r->h;
				changed = EINA_TRUE;
			}
		}
		break;
	case EKEKO_ATTRIBUTE_BOOL:
		if (v->v.b != n->attr.prev.v.b)
		{
			changed = EINA_TRUE;
			n->value.v.b = v->v.b;
		}
		break;
	/* TODO User provided ones */
	default:
		printf("no such type yet %d!!!\n", v->type);
		break;
	}
	if (changed == EINA_TRUE)
	{
		Ekeko_Event_Mutation *e;
		Ekeko_Element *owner;

		/* call the mutation event */
		e = (Ekeko_Event_Mutation *)ekeko_document_event_new(n->owner, "MutationEvents");
		owner = ekeko_attribute_element_get((Ekeko_Attribute *)n);
		ekeko_event_mutation_init(e, "DOMAttrModified", EINA_FALSE,
				EINA_FALSE, (Ekeko_Node *)owner, &old, v, n->name,
				EKEKO_EVENT_MUTATION_MODIFCATION);
		ekeko_node_event_dispatch((Ekeko_Node *)owner, (Ekeko_Event *)e);
		
	}
	n->changed = changed;
	return;
}
void _attribute_process(Ekeko_Node *n)
{
	Ekeko_Event_Mutation *e;
	Ekeko_Element *owner;

	if (!n->changed)
		return;
	
	/* call the mutation event */
	e = (Ekeko_Event_Mutation *)ekeko_document_event_new(n->owner, "MutationEvents");
	owner = ekeko_attribute_element_get((Ekeko_Attribute *)n);
	ekeko_event_mutation_init(e, "DOMAttrUpdated", EINA_FALSE, EINA_FALSE,
			(Ekeko_Node *)owner, &n->attr.prev, &n->value, n->name,
			EKEKO_EVENT_MUTATION_MODIFCATION);
	ekeko_node_event_dispatch((Ekeko_Node *)owner, (Ekeko_Event *)e);
	/* update prev */
	n->attr.prev = n->value;
	n->changed = EINA_FALSE;		
}
void _element_process(Ekeko_Node *n)
{
	Eina_Iterator *it;
	Ekeko_Node *in;

	//printf("[1  Element changed %d\n", n->changed);
	if (!n->changed)
		return;
	/* all attributes */
	it = eina_hash_iterator_data_new(n->attributes);
	while (eina_iterator_next(it, (void **)&in))
	{
		if (ekeko_node_changed(in))
		{
			_attribute_process(in);
			n->changed--;
		}
	}
	/* FIXME maybe on the above loop some DOMAttrUpdated has changed
	 * another attribute, if that's the case, return there */
	//printf(" 1] Element changed %d\n", n->changed);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_node_initialize(Ekeko_Node *n)
{
	n->user = eina_hash_string_superfast_new(NULL);
	n->attributes = NULL;
	n->parent = NULL;
	n->changed = 0;
#ifdef EKEKO_EVENT
	n->events = eina_hash_string_superfast_new(NULL);
#endif
}

Eina_Bool ekeko_node_changed(Ekeko_Node *n)
{
	if (n->changed > 0)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}
/*
 * The node should be an element as is the only one that has attributes
 */
void ekeko_node_attribute_set(Ekeko_Node *n, const char *name, Ekeko_Value *v)
{
	Ekeko_Attribute *a;
	Eina_Bool changed_bef, changed_now;
	
	a = eina_hash_find(n->attributes, name);
	if (!a)
	{
		a = ekeko_attribute_new((Ekeko_Element *)n, name, v);
		eina_hash_add(n->attributes, name, a);
		_node_change_rec(n, 1);

		return;
	}

	changed_bef = ekeko_node_changed((Ekeko_Node *)a);
	_attribute_value_set((Ekeko_Node *)a, v);
	changed_now = ekeko_node_changed((Ekeko_Node *)a);

	if (changed_bef && !changed_now)
	{
		_node_unchange_rec(n, 1);
	}
	else if (!changed_bef && changed_now)
	{
		_node_change_rec(n, 1);
	}
	if (changed_now)
	{
		//printf("Attribute %s changed now\n", name);
	}
	if (changed_bef)
	{
		//printf("Attribute %s changed before\n", name);
	}
}

void ekeko_node_attribute_get(Ekeko_Node *n, const char *name, Ekeko_Value *v)
{
	Ekeko_Attribute *a;

	a = eina_hash_find(n->attributes, name);
	if (!a) return;

	/* FIXME change this to _node_ */
	ekeko_attribute_value_get(a, v);
}
void ekeko_node_attribute_remove(Ekeko_Node *n, const char *name)
{
	Ekeko_Attribute *a;
	
	a = eina_hash_find(n->attributes, name);
	if (a)
	{
		eina_hash_del(n->attributes, name, NULL);
		/* TODO delete the attribute */
		/* check if it is changed if so, decrement the parent element */
		/* setup a default attribute */
	}	
}
Eina_Bool ekeko_node_event_dispatch(Ekeko_Node *n, Ekeko_Event *e)
{
	Ekeko_Event_Listener_Container *elc;
	Eina_Iterator *it;
	Eina_List *listeners;

	/* fill the common event values */
	e->current_target = n;
	/* get the event name and search it on the hash */
	listeners = eina_hash_find(n->events, e->type);
	/* iterate over the list and send events */
	//printf("Calling event %s for the listeners %p\n", e->type, listeners);
	it = eina_list_iterator_new(listeners);
	while (eina_iterator_next(it, (void **)&elc))
	{
		elc->listener(e);
	}

	return EINA_TRUE;
}

void ekeko_node_event_listener_add(Ekeko_Node *n, const char *type,
		Ekeko_Event_Listener el, Eina_Bool capture)
{
	Eina_List *events;
	Ekeko_Event_Listener_Container *elc;
	
	events = eina_hash_find(n->events, type);

	elc = malloc(sizeof(Ekeko_Event_Listener_Container));
	elc->capture = capture;
	elc->listener = el;

	if (events)
	{
		/* 
		 * TODO
		 * in case the same listener with the same parameters is called
		 * it should be discarded
		 */
		events = eina_list_append(events, elc);
		return;
	}
	else
	{
		events = eina_list_append(events, elc);
		//printf("events = %p, type = %s\n", events, type);
		eina_hash_add(n->events, type, events);
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * 
 */
EAPI void ekeko_node_value_get(Ekeko_Node *n, Ekeko_Value *v)
{
	*v = n->value;
}
/**
 * 
 * 
 */
EAPI void ekeko_node_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	switch (n->type)
	{
	case EKEKO_NODE_ATTRIBUTE:
		_attribute_value_set(n, v);
		break;
	}
}
/**
 * 
 */
EAPI Ekeko_Node_Type ekeko_node_type(Ekeko_Node *n)
{
	return n->type;
}
EAPI Ekeko_Node * ekeko_node_parent(Ekeko_Node *n)
{
	
}
EAPI Ekeko_Node_List * ekeko_node_childs_get(Ekeko_Node *n)
{
	
}
/* LEVEL 2 */
EAPI Ekeko_Document * ekeko_node_document_owner(Ekeko_Node *n)
{
	
}
/* LEVEL 3 */
EAPI void ekeko_node_user_set(Ekeko_Node *n, const char *key, void *data)
{
	eina_hash_add(n->user, key, data);
}
/**
 * 
 */
EAPI void * ekeko_node_user_get(Ekeko_Node *n, const char *key)
{
	eina_hash_find(n->user, key);
}
/**
 * 
 */
EAPI Ekeko_Node * ekeko_node_insert_before(Ekeko_Node *p,  Ekeko_Node *n, Ekeko_Node *ref)
{
	/* TODO check that n is part of p childs */ 
	
	p->childs = eina_inlist_prepend_relative(p->childs, EINA_INLIST_GET(ref), EINA_INLIST_GET(n));
	/* DOMInserted */
	
}
/**
 * 
 * 
 */
EAPI Ekeko_Node * ekeko_node_child_replace(Ekeko_Node *p, Ekeko_Node *n, Ekeko_Node *old)
{
	
}
/**
 * 
 */
EAPI Ekeko_Node * ekeko_node_child_remove(Ekeko_Node *p, Ekeko_Node *n, Ekeko_Node *old)
{
	/*DOMNodeRemoved*/
}
/**
 * 
 */
EAPI Ekeko_Node * ekeko_node_child_append(Ekeko_Node *p, Ekeko_Node *n)
{
	Ekeko_Event_Mutation *e = NULL;
	
	/* append the child */
	p->childs = eina_inlist_append(p->childs, EINA_INLIST_GET(n));
	if (n->parent)
	{
		/* FIXME handle the case of appending a node which
		 * already has a parent
		 */
		printf("Already has a parent\n");
	}
	n->parent = p;
	_node_change_rec(p, n->changed);
	/* DOMNodeInserted */
	if (p->type == EKEKO_NODE_DOCUMENT)
		e = (Ekeko_Event_Mutation *)ekeko_document_event_new((Ekeko_Document *)p, "MutationEvents");
	else if (p->type == EKEKO_NODE_ELEMENT)
		e = (Ekeko_Event_Mutation *)ekeko_document_event_new(p->owner, "MutationEvents");
	else
		return NULL;
	if (!e)
		return n;
	ekeko_event_mutation_init(e, "DOMNodeInserted", EINA_FALSE, EINA_FALSE,
			p, NULL, NULL, NULL,
			EKEKO_EVENT_MUTATION_ADDITION);
	ekeko_node_event_dispatch(n, (Ekeko_Event *)e);

	return n;
}

/* Own */
/* Split this function in two for the n->changed check */
EAPI void ekeko_node_process(Ekeko_Node *n)
{
	Eina_Iterator *it;
	Ekeko_Node *in;
	Ekeko_Event_Process *e = NULL;

	/* all childs */
	if (!n->changed)
		return;
	//printf("[0  Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* handle the attributes as they dont have any parent, childs or siblings */
	if (n->type == EKEKO_NODE_ELEMENT)
	{
		_element_process(n);
		if (!n->changed)
			return;
	}
	/* handle childs */
	it = eina_inlist_iterator_new(n->childs);
	while (eina_iterator_next(it, (void **)&in))
	{
		int changed = in->changed;
		
		if (!changed)
			continue;
		
		ekeko_node_process(in);
		n->changed -= changed;
		if (!n->changed)
		{
			break;
		}
	}
	//printf(" 0] Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* post condition */
	assert(!n->changed);
	if (n->type == EKEKO_NODE_DOCUMENT)
		e = (Ekeko_Event_Process *)ekeko_document_event_new((Ekeko_Document *)n, "ProcessEvents");
	else if (n->type == EKEKO_NODE_ELEMENT)
		e = (Ekeko_Event_Process *)ekeko_document_event_new(n->owner, "ProcessEvents");
	else
		return;
	ekeko_event_process_init(e, n, EKEKO_EVENT_PROCESS_PHASE_POST);
	ekeko_node_event_dispatch(n, (Ekeko_Event *)e);
}
