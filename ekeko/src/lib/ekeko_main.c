/*
 * etk2_main.c
 *
 *  Created on: 07-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _count;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI int ekeko_main_init(void)
{
	if (_count) goto done;
	eina_init();
	ekeko_value_init();
done:
	return ++_count;
}

EAPI int ekeko_main_shutdown(void)
{
	if (_count != 1) goto done;
	ekeko_value_shutdown();
	eina_shutdown();
done:
	return --_count;
	/* TODO */
}
