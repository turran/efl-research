/*
 * event.h
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

#ifndef EVENT_H_
#define EVENT_H_

void event_mutation_init(Ekeko_Event_Mutation *em, const char *type, const Ekeko_Object *o,
		const Ekeko_Object *rel, const Ekeko_Property *prop, Ekeko_Value *prev, Ekeko_Value *curr,
		Ekeko_Event_Mutation_State state);
void event_ui_init(Ekeko_Event_Ui *eui, const char *type, const Ekeko_Object *o,
		const Ekeko_Object *related, const Ekeko_Input *i);
void event_mouse_move_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i, unsigned int sx, unsigned int sy);
void event_mouse_in_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i);
void event_mouse_out_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i);
void event_mouse_down_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i);
void event_mouse_up_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i);
void event_mouse_click_init(Ekeko_Event_Mouse *em, const Ekeko_Object *o, const Ekeko_Object *related,
		const Ekeko_Input *i);

#endif /* EVENT_H_ */
