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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
typedef void (*Ekeko_Event_Listener)(Ekeko_Event *);

/**
 * Adds a listener for the event type ...
 */
EAPI void ekeko_event_listener_add(Ekeko_Element *e, const char *type,
		Ekeko_Event_Listener el, Eina_Bool capture)
{

}

EAPI void ekeko_event_listener_remove(Ekeko_Element *e, const char *type,
		Ekeko_Event_Listener el, Eina_Bool capture)
{

}

EAPI Eina_Bool ekeko_event_dispatch(Ekeko_Element *e, Ekeko_Event *ev)
{

}
