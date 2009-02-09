/*
 * etk2_engine_sdl.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
static Etk_Engine etk_engine_sdl = {
	.doc = &etk_document_engine_sdl,
	.canvas = &etk_canvas_engine_sdl,
	.context = &etk_context_engine_enesim,
	.shape = &etk_shape_engine_sdl,
};

void engine_sdl_init(void)
{
	etk_engine_register(&etk_engine_sdl, "sdl");
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

