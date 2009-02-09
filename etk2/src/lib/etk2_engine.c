/*
 * etk2_engine.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Hash *_engines = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void etk_engine_init(void)
{
	_engines = eina_hash_string_superfast_new(NULL);
#ifdef BUILD_ENGINE_SDL
	engine_sdl_init();
#endif
}

void etk_engine_shutdown(void)
{

}

void etk_engine_register(Etk_Engine *engine, const char *name)
{
	eina_hash_add(_engines, name, engine);
}

Etk_Engine * etk_engine_get(const char *name)
{
	return eina_hash_find(_engines, name);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* TODO later :)
EAPI void * etk_engine_canvas_create(Eina_Document *d, Eina_Bool root)
{

}
*/
