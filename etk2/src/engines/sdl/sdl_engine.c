/*
 * eon_engine_sdl.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
#include "Engine_SDL.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
//#define SDL_PURE
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
static Eon_Engine eon_engine_sdl = {
	.doc = &eon_document_engine_sdl,
#ifdef SDL_PURE
	.canvas = &eon_canvas_engine_sdl,
#else
	.canvas = &eon_canvas_engine_sdl_enesim,
#endif
	.context = &eon_context_engine_enesim,
#ifdef SDL_PURE
	.shape = &eon_shape_engine_sdl,
#else
	.shape = &eon_shape_engine_enesim,
#endif
};

void engine_sdl_init(void)
{
	eon_engine_register(&eon_engine_sdl, "sdl");
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

