/*
 * enesim_context.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#include "enesim_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void * _create(void)
{
	return calloc(1, sizeof(Context));
	//return enesim_context_new();
}

static void _delete(void *c)
{
	free(c);
	//enesim_context_delete(c);
}

static void _color_set(void *c, int color)
{
	Context *ctx = c;
	uint32_t cmul;
	uint8_t a = color >> 24;
	if (a != 256)
	{
		cmul = (color & 0xff000000) + (((((color) >> 8) & 0xff) * a) & 0xff00) +
			(((((color) & 0x00ff00ff) * a) >> 8) & 0x00ff00ff);
	}
	else
		color = cmul;
	ctx->color = color;
	//enesim_context_color_set(c, cmul);
}

static void _rop_set(void *c, int rop)
{
	Context *ctx = c;

	ctx->rop = rop;
	//enesim_context_rop_set(c, rop);
}

static void _matrix_set(void *c, Enesim_Matrix *m)
{
	//enesim_context_matrix_set(c, m);
}

static void _clip_set(void *c, Eina_Rectangle *r)
{
	Context *ctx = c;

	ctx->clip.rect = *r;
	ctx->clip.used = EINA_TRUE;
	//enesim_context_clip_set(c, r);
}

static void _clip_clear(void *c)
{
	Context *ctx = c;

	ctx->clip.used = EINA_FALSE;
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
	.clip_set = _clip_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

