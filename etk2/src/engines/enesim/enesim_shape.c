/*
 * enesim_shape.c
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
static void _rect(void *surface, void *context, int x, int y, int w, int h)
{
	Enesim_Surface *s = surface;
	Context *c = context;
	Enesim_Operator op;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t *src;
	Eina_Rectangle rectclip;
	Eina_Rectangle rarea;
	uint32_t stride;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING A RECTANGLE at %d %d %d %d to %p\n", x, y, w, h, s);
#endif
	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_color_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, c->color);

	//printf("Trying to render a rectangle at %d %d %d %d\n", x, y, w, h);
	/* the context has the clipping rect relative to the canvas */
#if 0
	eina_rectangle_coords_from(&rarea, x, y, w, h);
	rectclip = rarea;
	if (c->clip.used)
	{
		printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		eina_rectangle_rescale_in(&rarea, &c->clip.rect, &rectclip);
		printf("rescaled to %d %d %d %d\n", rectclip.x, rectclip.y, rectclip.w, rectclip.h);
	}
	stride = enesim_surface_stride_get(s);
	src = enesim_surface_data_get(s) + (rarea.y * stride) + rarea.x;
	while (rectclip.h--)
	{
		enesim_operator_drawer_span(&op, src, rectclip.w, NULL, c->color, NULL);
		src += stride;
	}
#endif
	stride = enesim_surface_stride_get(s);
	src = enesim_surface_data_get(s);
	src += (y * stride) + x;
	while (h--)
	{
		enesim_operator_drawer_span(&op, src, w, NULL, c->color, NULL);
		src += stride;
	}
	//enesim_rect_draw(s, c, x, y, w, h);
}

static void _image(void *surface, void *context, Enesim_Surface *src, Eina_Rectangle *srect)
{
	Enesim_Surface *dst = surface;
	Context *c = context;

	Enesim_Operator op;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t *ssrc, *ddst;
	uint32_t sstride, dstride;
	Eina_Rectangle imgclip;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING AN IMAGE\n");
#endif
	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	//printf("Trying to render an image at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	/* the context has the clipping rect relative to the canvas */
	imgclip = *srect;
	if (c->clip.used)
	{
		printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		eina_rectangle_rescale_in(srect, &c->clip.rect, &imgclip);
		printf("rescaled to %d %d %d %d\n", imgclip.x, imgclip.y, imgclip.w, imgclip.h);
	}
	sstride = enesim_surface_stride_get(src);
	dstride = enesim_surface_stride_get(dst);
	imgclip.x = imgclip.y = 0;
	ssrc = ((uint32_t *)enesim_surface_data_get(src)) + (imgclip.y * sstride) + imgclip.x;
	ddst = ((uint32_t *)enesim_surface_data_get(dst)) + (srect->y * dstride) + srect->x;
	while (imgclip.h--)
	{
		enesim_operator_drawer_span(&op, ddst, imgclip.w, ssrc, 0, NULL);
		ssrc += sstride;
		ddst += dstride;
	}
	//enesim_image_draw(dst, c, s, srect);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Shape_Engine etk_shape_engine_enesim = {
	.rect = _rect,
	.image = _image,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

