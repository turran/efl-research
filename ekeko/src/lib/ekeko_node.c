#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Ekeko_Node_Private
{
	EINA_INLIST;
	Ekeko_Node_Type type;
	const char *name;
	Ekeko_Value *value;
	/* used on attributes and elements, to know when some child or attribute
	 * has changed
	 */
	int changed;
	/* only used on attributes */
	struct
	{
		Ekeko_Value *prev; 
	} attr;
	Eina_Inlist *childs;
	Ekeko_Node *parent;
	Ekeko_Node_Named_Map *attributes;
	Ekeko_Document *owner;
	Eina_Hash *user;
	Eina_Hash *events;	
};


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

static Eina_Bool _int_value_cmp(Ekeko_Node *n, Ekeko_Value *v)
{
	int s;
	int p;

	s = ekeko_value_int_get(v);
	p = ekeko_value_int_get(n->attr.prev);

	if (s != p)
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static Eina_Bool _rectangle_value_cmp(Ekeko_Node *n, Ekeko_Value *v)
{
	Eina_Rectangle s;
	Eina_Rectangle p;
	
	ekeko_value_rectangle_get(v, &s);
	ekeko_value_rectangle_get(n->attr.prev, &p);
	
	if ((s.x != p.x) || (s.y != p.y) || (s.w != p.w) || (s.h != p.h))
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static Eina_Bool _bool_value_cmp(Ekeko_Node *n, Ekeko_Value *v)
{
	Eina_Bool s;
	Eina_Bool p;

	ekeko_value_bool_get(v, &s);
	ekeko_value_bool_get(&n->attr.prev, &p);
	
	if (s != p)
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static Eina_Bool _external_value_cmp(Ekeko_Node *n, Ekeko_Value *v)
{
	Ekeko_Value_Class *vclass;
	Ekeko_Value_Type type;
	void *s;

	type = ekeko_value_type_get(v);
	vclass = ekeko_value_class_get(type);
	s = ekeko_value_external_get(v);
	if (vclass->compare(n->attr.prev, v))
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

void _attribute_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	Eina_Bool changed = EINA_FALSE;
	Ekeko_Value_Type type;
	Ekeko_Value *old;

	/* FIXME check that the type is the same */
	type = ekeko_value_type_get(v);
	/* backup current value in case it is stored */
	old = n->value;
	switch (type)
	{
	/* First basic types */
	case EKEKO_ATTRIBUTE_NONE:
		printf("attribute %s hasnt been initialized\n", n->name);
		exit(1);
	case EKEKO_ATTRIBUTE_INT:
		changed = _int_value_cmp(n, v);
		break;
	case EKEKO_ATTRIBUTE_RECTANGLE:
		changed = _rectangle_value_cmp(n, v);
		break;
	case EKEKO_ATTRIBUTE_BOOL:
		changed = _bool_value_cmp(n, v);
		break;
	default:
		changed = _external_value_cmp(n, v);
		break;
	}
end:
	if (changed == EINA_TRUE)
	{
		Ekeko_Event_Mutation *e;
		Ekeko_Element *owner;

		/* replace the value */
		n->value = v;
		/* call the mutation event */
		e = (Ekeko_Event_Mutation *)ekeko_document_event_new(n->owner, "MutationEvents");
		owner = ekeko_attribute_element_get((Ekeko_Attribute *)n);
		ekeko_event_mutation_init(e, "DOMAttrModified", EINA_FALSE,
				EINA_FALSE, (Ekeko_Node *)owner, old, v, n->name,
				EKEKO_EVENT_MUTATION_MODIFCATION);
		ekeko_node_event_dispatch((Ekeko_Node *)owner, (Ekeko_Event *)e);
		ekeko_value_delete(old);
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
			(Ekeko_Node *)owner, n->attr.prev, n->value, n->name,
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
static void _node_initialize(Ekeko_Node *n, const char *name, Ekeko_Document *owner)
{
	n->user = eina_hash_string_superfast_new(NULL);
	n->attributes = NULL;
	n->parent = NULL;
	n->changed = 0;
	n->owner = owner;
	n->name = strdup(name);
#ifdef EKEKO_EVENT
	n->events = eina_hash_string_superfast_new(NULL);
#endif
}

static void _value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	Eina_Bool changed_bef, changed_now;

	changed_bef = ekeko_node_changed(n);
	_attribute_value_set(n, v);
	changed_now = ekeko_node_changed(n);

	if (changed_bef && !changed_now)
	{
		_node_unchange_rec(n, 1);
	}
	else if (!changed_bef && changed_now)
	{
		_node_change_rec(n, 1);
	}
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_node_attribute_new(Ekeko_Node *n, const char *name, Ekeko_Document *owner)
{
	_node_initialize(n, name, owner);
	n->type = EKEKO_NODE_ATTRIBUTE;
	n->value = ekeko_value_new(EKEKO_ATTRIBUTE_NONE, NULL);
	n->attr.prev = ekeko_value_new(EKEKO_ATTRIBUTE_NONE, NULL);
}

void ekeko_node_element_new(Ekeko_Node *n, const char *name, Ekeko_Document *owner)
{
	_node_initialize(n, name, owner);
	n->type = EKEKO_NODE_ELEMENT;
	n->attributes = eina_hash_string_superfast_new(NULL);
	
}

Eina_Bool ekeko_node_changed(Ekeko_Node *n)
{
	if (n->changed > 0)
		return EINA_TRUE;
	else
		return EINA_FALSE;
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
EAPI Ekeko_Value * ekeko_node_value_get(Ekeko_Node *n)
{
	return n->value;
}
/*EAPI void _attribute_value_set(Ekeko_Node *n, Ekeko_Value *v)
{
	
}*/
/**
 * 
 * 
 */
EAPI void ekeko_node_value_set(Ekeko_Node *n, const char *v)
{
	Ekeko_Value *val;

	if (n->type != EKEKO_NODE_ATTRIBUTE)
		return;

	val = ekeko_value_new(EKEKO_ATTRIBUTE_STRING, (void *)v);
	_value_set(n, val);
}
/**
 * 
 * 
 */
EAPI void ekeko_node_value_int_set(Ekeko_Node *n, int v)
{
	Ekeko_Value *val;

	if (n->type != EKEKO_NODE_ATTRIBUTE)
		return;

	val = ekeko_value_new(EKEKO_ATTRIBUTE_INT, &v);
	_value_set(n, val);	
}
/**
 * 
 * 
 * 
 */
EAPI void ekeko_node_value_bool_set(Ekeko_Node *n, Eina_Bool v)
{
	Ekeko_Value *val;

	if (n->type != EKEKO_NODE_ATTRIBUTE)
		return;

	val = ekeko_value_new(EKEKO_ATTRIBUTE_BOOL, &v);
	_value_set(n, val);
}
/**
 * 
 * 
 */
EAPI void ekeko_node_value_rectangle_set(Ekeko_Node *n, Eina_Rectangle *v)
{
	Ekeko_Value *val;

	if (n->type != EKEKO_NODE_ATTRIBUTE)
		return;

	val = ekeko_value_new(EKEKO_ATTRIBUTE_RECTANGLE, (void *)v);
	_value_set(n, val);
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
	Ekeko_Node_Private *np;
	Eina_Iterator *it;
	Ekeko_Node *in;
	Ekeko_Event_Process *e = NULL;

	np = n->p;
	/* all childs */
	if (!np->changed)
		return;
	//printf("[0  Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* handle the attributes as they dont have any parent, childs or siblings */
	if (np->type == EKEKO_NODE_ELEMENT)
	{
		_element_process(np);
		if (!np->changed)
			return;
	}
	/* handle childs */
	it = eina_inlist_iterator_new(np->childs);
	while (eina_iterator_next(it, (void **)&in))
	{
		int changed = in->changed;
		
		if (!changed)
			continue;
		
		ekeko_node_process(in);
		np->changed -= changed;
		if (!np->changed)
		{
			break;
		}
	}
	//printf(" 0] Node changed %d %s %d\n", n->type, n->name, n->changed);
	/* post condition */
	assert(!np->changed);
	if (np->type == EKEKO_NODE_DOCUMENT)
		e = (Ekeko_Event_Process *)ekeko_document_event_new((Ekeko_Document *)np, "ProcessEvents");
	else if (np->type == EKEKO_NODE_ELEMENT)
		e = (Ekeko_Event_Process *)ekeko_document_event_new(np->owner, "ProcessEvents");
	else
		return;
	ekeko_event_process_init(e, np, EKEKO_EVENT_PROCESS_PHASE_POST);
	ekeko_node_event_dispatch(np, (Ekeko_Event *)e);
}
