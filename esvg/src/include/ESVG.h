#ifndef _ESVG_H
#define _ESVG_H

#include "Eina.h"
#include "Ekeko.h"

#include "esvg_class.h"
#include "esvg_core.h"
#include "esvg_element.h"
#include "esvg_shape.h"
#include "esvg_container.h"
#include "esvg_rect.h"
#include "esvg_svg.h"
#include "esvg_canvas.h"

#include "esvg_parser.h"

/**
 * TODO 
 * + split the code in two, the drawing and the parser =)
 * + We need to handle two different kinds of backends, the drawer/context one
 * and the output, so we can use cairo image backend and a sdl output for
 * example.
 * + Make the parser optional
 * 
 */

#endif
