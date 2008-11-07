#ifndef _ESVG_H
#define _ESVG_H

#include "Eina.h"
#include "Ekeko.h"

#include "esvg_rect.h"

/*#include "esvg_dom.h"
#include "esvg_core.h"
#include "esvg_engine.h"
#include "esvg_element.h"
#include "esvg_document.h"
#include "esvg_shape.h"
#include "esvg_container.h"
#include "esvg_group.h"
#include "esvg_svg.h"

#include "esvg_parser.h"*/

/**
 * TODO 
 * + split the code in two, the drawing and the parser =)
 * + We need to handle two different kinds of backends, the drawer/context one
 * and the output, so we can use cairo image backend and a sdl output for
 * example.
 * + Make the parser optional
 * + Maybe the container should be extended by a "shape container", so containers
 * like the filter is possible and we can reuse some code for all the shape
 * containers
 */

#endif
