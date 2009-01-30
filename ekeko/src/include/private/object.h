/*
 * object.h
 *
 *  Created on: 30-ene-2009
 *      Author: jl
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

#ifndef OBJECT_H_
#define OBJECT_H_

Ekeko_Type * object_private_type_get(Ekeko_Object *object);
void object_construct(Ekeko_Type *type, void *instance);
void object_event_listener_add(Ekeko_Object *obj, const char *type, Event_Listener el, Eina_Bool bubble);
void object_event_listener_remove(Ekeko_Object *obj, const char *type, Event_Listener el);

#endif /* OBJECT_H_ */
