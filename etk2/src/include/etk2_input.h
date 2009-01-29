/*
 * etk2_input.h
 *
 *  Created on: 28-ene-2009
 *      Author: jl
 */

#ifndef ETK2_INPUT_H_
#define ETK2_INPUT_H_

EAPI void input_feed_mouse_in(Input *i);
EAPI void input_feed_mouse_move(Input *i, unsigned int x, unsigned int y);
EAPI void input_feed_mouse_out(Input *i);
EAPI void input_feed_mouse_down(Input *i);
EAPI void input_feed_mouse_up(Input *i);

#endif /* ETK2_INPUT_H_ */
