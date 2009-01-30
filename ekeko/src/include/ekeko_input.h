/*
 * etk2_input.h
 *
 *  Created on: 28-ene-2009
 *      Author: jl
 */

#ifndef EKEKO_INPUT_H_
#define EKEKO_INPUT_H_

EAPI void ekeko_input_feed_mouse_in(Ekeko_Input *i);
EAPI void ekeko_input_feed_mouse_move(Ekeko_Input *i, unsigned int x, unsigned int y);
EAPI void ekeko_input_feed_mouse_out(Ekeko_Input *i);
EAPI void ekeko_input_feed_mouse_down(Ekeko_Input *i);
EAPI void ekeko_input_feed_mouse_up(Ekeko_Input *i);

#endif /* ETK2_INPUT_H_ */
