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
#define RADDIST 1
static void _rect(void *surface, void *context, int x, int y, int w, int h)
{
	Enesim_Surface *s = surface;
	Context *c = context;
	Enesim_Operator op;
	Enesim_Operator f;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t *src;
	uint32_t *src2;
	uint32_t sw, sh, fw, fh;
	Eina_Rectangle rectclip;
	uint32_t stride, fstride;
	uint32_t *span;
	uint32_t *fsrc;
	int fy;
	Enesim_Surface *ftmp = NULL;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING A RECTANGLE at %d %d %d %d to %p\n", x, y, w, h, s);
#endif
	cpus = enesim_cpu_get(&numcpus);
#if 0
if (c->color == 0xffffffff)
{
#endif
	/* the context has the clipping rect relative to the canvas */
	enesim_drawer_span_color_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, c->color);
	//printf("Trying to render a rectangle at %d %d %d %d\n", x, y, w, h);
	if (c->clip.used)
	{
		//printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		//eina_rectangle_rescale_in(&rarea, &c->clip.rect, &rectclip);
		//printf("rescaled to %d %d %d %d\n", rectclip.x, rectclip.y, rectclip.w, rectclip.h);
		rectclip = c->clip.rect;
	}
	else
	{
		eina_rectangle_coords_from(&rectclip, x, y, w, h);
	}
	stride = enesim_surface_stride_get(s);
	src = ((uint32_t *)enesim_surface_data_get(s)) + (rectclip.y * stride) + rectclip.x;
	while (rectclip.h--)
	{
		enesim_operator_drawer_span(&op, src, rectclip.w, NULL, c->color, NULL);
		src += stride;
	}
}
#if 0
else {
	enesim_drawer_span_pixel_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);
#if RADDIST
	enesim_raddist_1d_op_get(&f, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_FAST, ENESIM_FORMAT_ARGB8888);
#else
	enesim_dispmap_1d_op_get(&f, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_FAST, ENESIM_FORMAT_ARGB8888);
	{
		emage_load("/home/jl/sphere.png", &ftmp, NULL);
		if (!ftmp)
			return;
		fsrc = enesim_surface_data_get(ftmp);
		fstride = enesim_surface_stride_get(ftmp);
		enesim_surface_size_get(ftmp, &fw, &fh);
		printf("SHERE %d %d %d\n", fw, fh, fstride);
	}
#endif
	if (c->clip.used)
	{
		//printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		//eina_rectangle_rescale_in(&rarea, &c->clip.rect, &rectclip);
		//printf("rescaled to %d %d %d %d\n", rectclip.x, rectclip.y, rectclip.w, rectclip.h);
		rectclip = c->clip.rect;
	}
	else
	{
		eina_rectangle_coords_from(&rectclip, x, y, w, h);
	}
	stride = enesim_surface_stride_get(s);
	src = src2 = enesim_surface_data_get(s);
	enesim_surface_size_get(s, &sw, &sh);
	src += (rectclip.y * stride) + rectclip.x;
	span = malloc(sizeof(uint32_t) * rectclip.w);
#if RADDIST
	fy = y;
#else
	fy = 0;
#endif
	while (rectclip.h--)
	{
#if RADDIST
		enesim_operator_raddist_1d(&f, src2, stride, sw, sh,
				x + (w / 2), y + (h / 2), hypot(w, h), 0,
				0, fy, rectclip.w, span);
		enesim_operator_drawer_span(&op, src, rectclip.w, span, c->color, NULL);
#else
		enesim_operator_dispmap_1d(&f, src2, stride, sw, sh,
				1, fsrc,
				0, fy, rectclip.w < fw ? rectclip.w : fw, span);
		enesim_operator_drawer_span(&op, src, rectclip.w < fw ? rectclip.w : fw, span, c->color, NULL);
#endif
		fy++;
		src += stride;
#if !RADDIST
		fsrc += fstride;
#endif
	}
	free(span);
#if !RADDIST
	if (ftmp)
		enesim_surface_delete(ftmp);
#endif
}
#endif

/* 
 * TODO we should use the SCALER2D
 * TODO use another scale qualities
 * render an image just scaling it
 */
static void _image_scale(Enesim_Surface *dst, void *context, Enesim_Surface *src, Eina_Rectangle *srect)
{
	Context *c = context;
	Enesim_Operator drawer, scaler;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t sstride, dstride;
	uint32_t *s, *d, *tmp;
	uint32_t sw, sh;
	uint32_t dw, dh;
	int y;
	Eina_Rectangle drect;
	
	s = enesim_surface_data_get(src);
	sstride = enesim_surface_stride_get(src);
	enesim_surface_size_get(src, &sw, &sh);
        d = enesim_surface_data_get(dst);
	dstride = enesim_surface_stride_get(dst);
	enesim_surface_size_get(dst, &dw, &dh);
	eina_rectangle_coords_from(&drect, 0, 0, dw, dh);

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_op_get(cpus[0], &drawer, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);
	enesim_scaler_1d_op_get(&scaler, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	//printf("S = %d %d D = %d %d\n", sw, sh, dw, dh);
	//printf("DRECT0 %d %d %d %d\n", drect.x, drect.y, drect.w, drect.h);
	if (c->clip.used)
	{
		eina_rectangle_rescale_in(&drect, &c->clip.rect, &drect);
	}
	//printf("DRECT1 %d %d %d %d\n", drect.x, drect.y, drect.w, drect.h);
	//printf("SRECT %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	d = d + (drect.y * dstride) + drect.x;
	/* TODO offset the source to the destination clipping */
	//s = s + (srect->y * sstride) + srect->x;
	y = 0;
	tmp = malloc(sizeof(uint32_t) * drect.w);
	while (y < drect.h)
	{
		uint32_t sy;
		uint32_t rs;

		sy = (y * sh) / srect->h;
		rs = s + (sy * sstride);	
		enesim_operator_scaler_1d(&scaler, rs, sw, 0,
                                        drect.w, srect->w, tmp);
		enesim_operator_drawer_span(&drawer, d, drect.w, tmp, 0, NULL);
		/* increment the source by the scale factor */
		d += dstride;
		y++;
	}
	free(tmp);
#if 0

	Eina_Rectangle imgclip;
	uint32_t sw, sh;

	printf("[Etk_Engine_Enesim] RENDERING AN IMAGE %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	enesim_surface_size_get(src, &sw, &sh);
	printf("SURFACE %d %d - %d %d\n", sw, sh, srect->w, srect->h);
	/* TODO get the quality */
	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	imgclip = *srect;
	if (c->clip.used)
	{
		//printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		eina_rectangle_rescale_in(srect, &c->clip.rect, &imgclip);
		//printf("rescaled to %d %d %d %d\n", imgclip.x, imgclip.y, imgclip.w, imgclip.h);
		//printf("old at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	}

	ssrc = enesim_surface_data_get(src);
	ssrc += (imgclip.y * sstride) + imgclip.x;
	ddst = enesim_surface_data_get(dst);
	ddst +=  (c->clip.rect.y * dstride) + c->clip.rect.x;
	while (imgclip.h--)
	{
		enesim_operator_drawer_span(&op, ddst, imgclip.w, ssrc, 0, NULL);
		ssrc += sstride;
		ddst += dstride;
	}
#endif
}

/*
 * surface: destintation surface
 * context: destination clipping?
 * srect: source rectangle (scaling??)
 * src: src image
 */
void etk2_enesim_image(void *surface, void *context, Enesim_Surface *src, Eina_Rectangle *srect)
{
	Enesim_Surface *dst = surface;
	Context *c = context;

	Enesim_Operator op;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t *ssrc, *ddst;
	uint32_t sstride, dstride;
	Eina_Rectangle imgclip;
	uint32_t sw, sh;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING AN IMAGE %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
#endif
	enesim_surface_size_get(src, &sw, &sh);
	/* downscale or upscale the image */
	if ((srect->w != sw) || (srect->h != sh))
	{
		printf("SURFACE %d %d - %d %d\n", sw, sh, srect->w, srect->h);
		_image_scale(dst, context, src, srect);
		return;
	}
	/* TODO get the quality */
	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	//printf("Trying to render an image at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	/* the context has the clipping rect relative to the canvas */
	imgclip = *srect;
	if (c->clip.used)
	{
		//printf("using clip at %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		eina_rectangle_rescale_in(srect, &c->clip.rect, &imgclip);
		//printf("rescaled to %d %d %d %d\n", imgclip.x, imgclip.y, imgclip.w, imgclip.h);
		//printf("old at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	}
	if (c->matrix.used)
	{
		int w, h;
		Enesim_Operator tx;
		uint32_t *stmp;

		if (!enesim_transformer_1d_op_get(&tx, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_MATRIX_AFFINE,
			 ENESIM_FAST, ENESIM_FORMAT_ARGB8888))
		{
			//printf("no transformer available\n");
			return;
		}
		else
		{
			//printf("transformer used %d %d!!!!!\n", imgclip.x, imgclip.y);
			//printf("%f %f %f\n", c->matrix.m.xx, c->matrix.m.xy, c->matrix.m.xz);
			//printf("%f %f %f\n", c->matrix.m.yx, c->matrix.m.yy, c->matrix.m.yz);
			//printf("%f %f %f\n", c->matrix.m.zx, c->matrix.m.zy, c->matrix.m.zz);
		}
		enesim_surface_size_get(src, &w, &h);
		stmp = malloc(imgclip.w * sizeof(uint32_t));
		sstride = enesim_surface_stride_get(src);
		dstride = enesim_surface_stride_get(dst);
		ssrc = enesim_surface_data_get(src);
		ddst = enesim_surface_data_get(dst);
		ddst +=  (c->clip.rect.y * dstride) + c->clip.rect.x;

		//printf("%d %d %d\n", w, h, sstride);
		//printf("old at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
		while (imgclip.h--)
		{
			int i;

			enesim_operator_transformer_1d(&tx, ssrc,
					sstride, w, h,
					0, 0,
					c->matrix.m.xx, c->matrix.m.xy, c->matrix.m.xz,
					c->matrix.m.yx, c->matrix.m.yy, c->matrix.m.yz,
					c->matrix.m.zx, c->matrix.m.zy, c->matrix.m.zz,
					imgclip.x, imgclip.y++, imgclip.w, stmp);
			enesim_operator_drawer_span(&op, ddst, imgclip.w, stmp, 0, NULL);
			ddst += dstride;
		}
		free(stmp);
	}
	else
	{
		sstride = enesim_surface_stride_get(src);
		dstride = enesim_surface_stride_get(dst);
		ssrc = enesim_surface_data_get(src);
		ssrc += (imgclip.y * sstride) + imgclip.x;
		ddst = enesim_surface_data_get(dst);
		ddst +=  (c->clip.rect.y * dstride) + c->clip.rect.x;
		while (imgclip.h--)
		{
			enesim_operator_drawer_span(&op, ddst, imgclip.w, ssrc, 0, NULL);
			ssrc += sstride;
			ddst += dstride;
		}
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Shape_Engine etk_shape_engine_enesim = {
	.rect = _rect,
	.image = etk2_enesim_image,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

