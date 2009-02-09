/*
 * etk2_main.c
 *
 *  Created on: 07-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _count = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI int etk_init(void)
{
	if (_count) goto done;
		eina_init();
		ecore_init();
		ekeko_init();
		etk_value_init();
		etk_engine_init();
done:
	return ++_count;
}

EAPI int etk_shutdown(void)
{
	if (_count != 1) goto done;
		etk_engine_shutdown();
		etk_value_shutdown();
		ekeko_shutdown();
		ecore_shutdown();
		eina_shutdown();
done:
	return --_count;
}

EAPI void etk_loop(void)
{
	ecore_main_loop_begin();
}
