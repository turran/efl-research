#include "Ekeko.h"
#include "ekeko_private.h"
/*Events:
the event target interface is an interface that every node should implement
event_listener_add(Element *, string type, listener, capture);
event_listener_remove(Element *, string type, listener, capture);
event_dispatch(Element *, Event *);

event listener = void callback(Event *)
event {
	string type;
	Event_Target * element;
	init function()
	...
}

document_create_event: creates an event as if the implementation has created it,
that means that the init function part of the event should be called after this
Core event types:
mutation: when the document structure has changed (attributes, text or name)
pre-post mmutation??

*/
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ekeko_Event * _mutation_new(void)
{
	Ekeko_Event_Mutation *e;
	
	e = malloc(sizeof(Ekeko_Event_Mutation));

	return (Ekeko_Event *)e;
}
static void _mutation_del(Ekeko_Event *e)
{
	free(e);
}

static Ekeko_Event * _process_new(void)
{
	Ekeko_Event_Process *e;
		
	e = malloc(sizeof(Ekeko_Event_Process));

	return (Ekeko_Event *)e;
}
static void _process_del(Ekeko_Event *e)
{
	free(e);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 * Setup the core events
 */
void ekeko_event_setup(Ekeko_Document_Type *dt)
{
	ekeko_document_type_event_register(dt, "MutationEvents", _mutation_new, _mutation_del);
	ekeko_document_type_event_register(dt, "ProcessEvents", _process_new, _process_del);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Adds a listener for the event type ...
 */
EAPI void ekeko_event_listener_add(Ekeko_Node *e, const char *type,
		Ekeko_Event_Listener el, Eina_Bool capture)
{
	ekeko_node_event_listener_add(e, type, el, capture);
}

EAPI void ekeko_event_listener_remove(Ekeko_Node *e, const char *type,
		Ekeko_Event_Listener el, Eina_Bool capture)
{

}

EAPI Eina_Bool ekeko_event_dispatch(Ekeko_Node *e, Ekeko_Event *ev)
{
	//ekeko_node_event_dispatch(e, ev);
}

/**
 *  Initialize an event
 */
EAPI void ekeko_event_init(Ekeko_Event *e, const char *type, Eina_Bool can_bubble,
		Eina_Bool can_cancel)

{
	e->type = strdup(type);
}
EAPI void ekeko_event_mutation_init(Ekeko_Event_Mutation *e, const char *type,
		Eina_Bool bubble, Eina_Bool cancelable, Ekeko_Node *rel,
		Ekeko_Value *prev, Ekeko_Value *curr, const char *attr,
		Ekeko_Event_Mutation_Change change)
{
	ekeko_event_init((Ekeko_Event *)e, type, bubble, cancelable);
	if (attr)
		e->attr = strdup(attr);
	e->change = change;
	e->curr = curr;
	e->prev = prev;
	e->related = rel;
}
EAPI void ekeko_event_process_init(Ekeko_Event_Process *e, Ekeko_Node *rel, Ekeko_Event_Process_Phase p)
{
	ekeko_event_init((Ekeko_Event *)e, "ProcessEvents", EINA_FALSE, EINA_FALSE);
	e->related = rel;
	e->phase = p;
}
