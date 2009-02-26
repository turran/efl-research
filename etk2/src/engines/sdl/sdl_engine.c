/*
 * etk2_engine_sdl.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#include "Engine_SDL.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
//#define SDL_PURE
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
static Etk_Engine etk_engine_sdl = {
	.doc = &etk_document_engine_sdl,
#ifdef SDL_PURE
	.canvas = &etk_canvas_engine_sdl,
#else
	.canvas = &etk_canvas_engine_sdl_enesim,
#endif
	.context = &etk_context_engine_enesim,
#ifdef SDL_PURE
	.shape = &etk_shape_engine_sdl,
#else
	.shape = &etk_shape_engine_enesim,
#endif
};

void engine_sdl_init(void)
{
	etk_engine_register(&etk_engine_sdl, "sdl");
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

