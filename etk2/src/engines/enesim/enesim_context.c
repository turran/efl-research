/*
 * enesim_context.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void * _create(void)
{
	return enesim_context_new();
}

static void _delete(void *c)
{
	enesim_context_delete(c);
}

static void _color_set(void *c, int color)
{
	Enesim_Context *ctx = c;

	enesim_context_color_set(c, color);
}

static void _rop_set(void *c, int rop)
{
	Enesim_Context *ctx = c;

	enesim_context_rop_set(c, rop);
}

static void _matrix_set(void *c, Enesim_Matrix *m)
{
	enesim_context_matrix_set(c, m);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Context_Engine etk_context_engine_enesim = {
	.create = _create,
	.delete = _delete,
	.color_set = _color_set,
	.rop_set = _rop_set,
	.matrix_set = _matrix_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

