/*
 * etk2_color.h
 *
 *  Created on: 04-mar-2009
 *      Author: jl
 */

#ifndef ETK2_COLOR_H_
#define ETK2_COLOR_H_

typedef uint32_t Etk_Color;

static inline void etk_color_set(Etk_Color *c, uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	*c = (a << 24) | (r << 16) | (g << 8) | b;
}

#endif /* ETK2_COLOR_H_ */
