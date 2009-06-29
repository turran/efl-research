#include "Eon.h"
#include "eon_private.h"

#include "Eon_Enesim.h"

#include <limits.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Engine_Enesim_Private
{

};

typedef struct _Context
{
	uint32_t color;
	Enesim_Rop rop;
	struct {
		Eina_Rectangle rect;
		Eina_Bool used;
	} clip;
	struct {
		Enesim_Matrix m;
		Eina_Bool used;
	} matrix;

} Context;

typedef struct _Enesim_Polygon
{
	Enesim_Rasterizer *r;
} Enesim_Polygon;

typedef struct _Enesim_Polygon_Data
{
	Enesim_Surface *dst;
	Enesim_Operator op;
	Context *c;
} Enesim_Polygon_Data;


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
	Context *ctx = c;

	ctx->matrix.used = EINA_TRUE;
	ctx->matrix.m = *m;
	//enesim_context_matrix_set(c, m);
}

static void _clip_set(void *c, Eina_Rectangle *r)
{
	Context *ctx = c;

	//printf("0 setting clip to %d %d %d %d\n", r->x, r->y, r->w, r->h);
	ctx->clip.rect = *r;
	ctx->clip.used = EINA_TRUE;
	//enesim_context_clip_set(c, r);
}

static void _clip_clear(void *c)
{
	Context *ctx = c;

	ctx->clip.used = EINA_FALSE;
	//printf("1 unsetting clip\n");
}
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

#if EON_DEBUG
	printf("[Eon_Eon_Engine_Enesim] RENDERING A RECTANGLE at %d %d %d %d to %p\n", x, y, w, h, s);
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


static void * _polygon_new(void)
{
	Enesim_Polygon *p = calloc(1, sizeof(Enesim_Polygon));

	return p;
}

void _polygon_point_add(void *polygon, int x, int y)
{
	Enesim_Polygon *p = polygon;

	/* create a rasterizer */
	/* TODO check the quality to create a kiia or a cpsc rasterizer */
	if (!p->r)
	{
		p->r = enesim_rasterizer_cpsc_new();
	}
	/* add a vertex to it */
	enesim_rasterizer_vertex_add(p->r, x, y);
}

void _rasterizer_cb(Enesim_Scanline *sl, void *data)
{
	Enesim_Polygon_Data *pdata = data;
	uint32_t *dst;

	dst = enesim_surface_data_get(pdata->dst);
	dst += sl->data.alias.y * (enesim_surface_stride_get(pdata->dst)) + sl->data.alias.x;
	//printf("rendering the polygon into %p!!!\n", data);
	enesim_operator_drawer_span(&pdata->op, dst, sl->data.alias.w, NULL, pdata->c->color, NULL);
}

void _polygon_draw(void *surface, void *context, void *polygon)
{
	Enesim_Polygon *p = polygon;
	Enesim_Cpu **cpus;
	Context *c = context;
	Enesim_Operator op;
	Enesim_Polygon_Data data;
	int numcpus;

	/* create the rendering pipeline */
	cpus = enesim_cpu_get(&numcpus);
	/* the context has the clipping rect relative to the canvas */
	/* TODO enesim should support the bounding box not only at creation time */
	/* TODO the pipeline can be whatever, not only color */
	enesim_drawer_span_color_op_get(cpus[0], &data.op, c->rop, ENESIM_FORMAT_ARGB8888, c->color);
	/* rasterize the polygon */
	data.dst = surface;
	data.c = context;
	enesim_rasterizer_generate(p->r, &c->clip.rect, _rasterizer_cb, &data);
}

/* Function used to draw a pattern whenever the rendering of an object isnt ready yet
 * like when an image isnt loaded yet
 */
static void _pattern_draw(Enesim_Surface *dst, void *context, Eina_Rectangle *darea)
{
}

static inline void _image_setup(Enesim_Surface *src, uint32_t **s,
		uint32_t *sstride, Enesim_Surface *dst, uint32_t **d,
	 	uint32_t *dstride)
{
	*s = enesim_surface_data_get(src);
	*sstride = enesim_surface_stride_get(src);

	*d = enesim_surface_data_get(dst);
	*dstride = enesim_surface_stride_get(dst);
}

/*
 * TODO we should use the SCALER2D
 * TODO use another scale qualities
 * render an image just scaling it
 * FIXME in case the rop is fill dont call the operator
 */
static void _image_scale(Enesim_Surface *dst, Eina_Rectangle *dclip,
		Context *c, Enesim_Surface *src, uint32_t sw, uint32_t sh,
		Eina_Rectangle *sgeom, Eina_Rectangle *sclip)
{
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Operator drawer, scaler;
	uint32_t sstride, dstride;
	uint32_t *s, *d, *tmp;
	int y;

	//printf("Scaling image from %d %d to %d %d\n", sw, sh, sgeom->w, sgeom->h);
	_image_setup(src, &s, &sstride, dst, &d, &dstride);

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_color_op_get(cpus[0], &drawer, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888, c->color);
	enesim_scaler_1d_op_get(&scaler, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	//printf("SGEOM %d %d %d %d\n", sgeom->x, sgeom->y, sgeom->w, sgeom->h);
	//printf("SCLIP %d %d %d %d\n", sclip.x, sclip.y, sclip.w, sclip.h);
	//printf("ENDING\n");
	d = d + (dclip->y * dstride) + dclip->x;
	/* offset the source in x */
	s = s + ((sclip->x * sw) / sgeom->w);
	y = 0;
	tmp = malloc(sizeof(uint32_t) * dclip->w);
	while (y < dclip->h)
	{
		uint32_t sy;
		uint32_t *rs;

		/* pick the to-scaled line */
		sy = ((sclip->y + y) * sh) / sgeom->h;
		rs = s + (sy * sstride);
		enesim_operator_scaler_1d(&scaler, rs, sw, 0,
				dclip->w, sgeom->w, tmp);
		enesim_operator_drawer_span(&drawer, d, dclip->w, tmp, c->color, NULL);
		/* increment the source by the scale factor */
		d += dstride;
		y++;
	}
	free(tmp);
}


static void _image_noscale(Enesim_Surface *dst, Eina_Rectangle *dclip, Context *c, Enesim_Surface *src, Eina_Rectangle *sgeom, Eina_Rectangle *sclip)
{
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Operator op;
	uint32_t sstride, dstride;
	uint32_t *s, *d;
	int y;
	/* TODO get the quality */

	_image_setup(src, &s, &sstride, dst, &d, &dstride);

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_color_op_get(cpus[0], &op, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888, c->color);

	s = s + (sclip->y * sstride) + sclip->x;
	d = d + (dclip->y * dstride) + dclip->x;
	while (dclip->h--)
	{
		enesim_operator_drawer_span(&op, d, dclip->w, s, c->color, NULL);
		s += sstride;
		d += dstride;
	}
}

/* As enesim doesnt support prescaling on the fly while doing transformations
 * we need to create a scaled image before transforming
 */
static void _image_transform(Enesim_Surface *dst, Eina_Rectangle *dclip, Context *c, Enesim_Surface *src, uint32_t sw, uint32_t sh, Eina_Rectangle *sgeom, Eina_Rectangle *sclip)
{
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Operator drawer, tx;
	uint32_t sstride, dstride;
	uint32_t *s, *d, *t;
	Enesim_Surface *realsrc = src;
	int y;

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_color_op_get(cpus[0], &drawer, c->rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888, c->color);
	enesim_transformer_1d_op_get(&tx, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_MATRIX_AFFINE,  ENESIM_FAST, ENESIM_FORMAT_ARGB8888);

	//printf("[%f %f %f]\n", c->matrix.m.xx, c->matrix.m.xy, c->matrix.m.xz);
	//printf("[%f %f %f]\n", c->matrix.m.yx, c->matrix.m.yy, c->matrix.m.yz);
	//printf("[%f %f %f]\n", c->matrix.m.zx, c->matrix.m.zy, c->matrix.m.zz);

	/* setup a temporary surface */
	/* downscale or upscale the image */
	if (((sgeom->w != sw) || (sgeom->h != sh)))
	{
		Enesim_Surface *tmp;
		Eina_Rectangle rclip;
		Context rc;

		rc.rop = ENESIM_FILL;
		rc.color = 0xffffffff;

		tmp = enesim_surface_new(ENESIM_FORMAT_ARGB8888, sgeom->w, sgeom->h);
		eina_rectangle_coords_from(&rclip, 0, 0, sgeom->w, sgeom->h);
		_image_scale(tmp, &rclip, &rc, src, sw, sh, &rclip, &rclip);
		realsrc = tmp;
	}
	_image_setup(realsrc, &s, &sstride, dst, &d, &dstride);

	t = malloc(dclip->w * sizeof(uint32_t));
	d =  d + (dclip->y * dstride) + dclip->x;
	y = 0;

	//printf("DCLIP %d %d %d %d\n", dclip->x, dclip->y, dclip->w, dclip->h);
	//printf("SGEOM %d %d %d %d\n", sgeom->x, sgeom->y, sgeom->w, sgeom->h);
	//printf("SCLIP %d %d %d %d\n", sclip->x, sclip->y, sclip->w, sclip->h);
	//printf("%d %d %d\n", w, h, sstride);
	//printf("old at %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	while (y < dclip->h)
	{
		memset(t, 0, sizeof(uint32_t) * dclip->w);
#if !DEBUG
		enesim_operator_transformer_1d(&tx, s,
				sstride, sgeom->w, sgeom->h,
				0, 0,
				c->matrix.m.xx, c->matrix.m.xy, c->matrix.m.xz,
				c->matrix.m.yx, c->matrix.m.yy, c->matrix.m.yz,
				c->matrix.m.zx, c->matrix.m.zy, c->matrix.m.zz,
				0, y, dclip->w, t);
		enesim_operator_drawer_span(&drawer, d, dclip->w, t, c->color, NULL);
		d += dstride;
#else
		enesim_operator_drawer_span(&drawer, d, dclip->w, s, 0, NULL);
		s += sstride;
		d += dstride;
#endif
		y++;
	}
	/* clean up */
	free(t);
	/* the temporary surface */
	if (realsrc != src)
	{
		enesim_surface_delete(realsrc);
	}
}



/*
 * surface: destintation surface
 * context: destination clipping?
 * srect: source rectangle (scaling??)
 * src: src image
 */
void eon_enesim_image(void *surface, void *context, Enesim_Surface *src, Eina_Rectangle *srect)
{
	Enesim_Surface *dst = surface;
	Context *c = context;
	Eina_Rectangle dclip, sclip;
	uint32_t sw, sh;
	uint32_t dw, dh;

#ifdef EON_DEBUG
	printf("[Eon_Eon_Engine_Enesim] RENDERING AN IMAGE %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
#endif
	enesim_surface_size_get(src, &sw, &sh);
	enesim_surface_size_get(dst, &dw, &dh);
	eina_rectangle_coords_from(&dclip, srect->x, srect->y, srect->w, srect->h);

	/* the context has the clipping rect relative to the canvas */
	//printf("ENTERING\n");
	//printf("S = %d %d D = %d %d\n", sw, sh, dw, dh);
	//printf("SGEOM %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	//printf("DCLIP %d %d %d %d\n", dclip.x, dclip.y, dclip.w, dclip.h);
	if (c->clip.used)
	{
		//printf("CLIP %d %d %d %d\n", c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
		eina_rectangle_rescale_in(srect, &c->clip.rect, &sclip);
		eina_rectangle_coords_from(&dclip, c->clip.rect.x, c->clip.rect.y, c->clip.rect.w, c->clip.rect.h);
	}
	//printf("SCLIP %d %d %d %d\n", sclip.x, sclip.y, sclip.w, sclip.h);
	//printf("DCLIP %d %d %d %d\n", dclip.x, dclip.y, dclip.w, dclip.h);

	/* transformed matrix */
	if (c->matrix.used)
	{
		//printf("Transforming image  %d %d - %d %d\n", sw, sh, srect->w, srect->h);
		_image_transform(dst, &dclip, c, src, sw, sh, srect, &sclip);
	}
	// TODO filter
	/* downscale or upscale the image */
	else if (((srect->w != sw) || (srect->h != sh)))
	{
		//printf("Scaling image  %d %d - %d %d\n", sw, sh, srect->w, srect->h);
		_image_scale(dst, &dclip, c, src, sw, sh, srect, &sclip);
		return;
	}
	/* no scale */
	else
	{
		//printf("No scaling image  %d %d - %d %d\n", sw, sh, srect->w, srect->h);
		_image_noscale(dst, &dclip, c, src, srect, &sclip);
	}
}

/*============================================================================*
 *                                   Rect                                     *
 *============================================================================*/
static void * rect_create(Eon_Rect *r)
{
	return r;
}

static void rect_render(void *er, void *c, Eina_Rectangle *clip)
{
	Eon_Rect *r = er;
	Enesim_Surface *s = c;
	Enesim_Operator op;
	Enesim_Operator f;
	Enesim_Cpu **cpus;
	int numcpus;
	uint32_t *src;
	uint32_t *src2;
	uint32_t sw, sh, fw, fh;
	uint32_t stride, fstride;
	uint32_t *span;
	uint32_t *fsrc;
	int fy;
	Enesim_Surface *ftmp = NULL;
	Eon_Color color;
	Enesim_Rop rop;

#if EON_DEBUG
	printf("[Eon_Eon_Engine_Enesim] RENDERING A RECTANGLE at %d %d %d %d to %p\n", clip.x, clip.y, clip.w, clip.h, s);
#endif
	cpus = enesim_cpu_get(&numcpus);
	color = eon_rect_color_get(r);
	rop = eon_rect_rop_get(r);
	enesim_drawer_span_color_op_get(cpus[0], &op, rop, ENESIM_FORMAT_ARGB8888, color);

	stride = enesim_surface_stride_get(s);
	src = ((uint32_t *)enesim_surface_data_get(s)) + (clip->y * stride) + clip->x;
	while (clip->h--)
	{
		enesim_operator_drawer_span(&op, src, clip->w, NULL, color, NULL);
		src += stride;
	}

}
/*============================================================================*
 *                                 Circle                                     *
 *============================================================================*/
typedef struct Circle
{
	Eon_Circle *c;
	Enesim_Rasterizer *r;
} Circle;

static void * circle_create(Eon_Circle *ec)
{
	Circle *c;

	c = malloc(sizeof(Circle));
	c->c = ec;
	c->r = enesim_rasterizer_circle_new();
	enesim_rasterizer_circle_fill_policy_set(c->r, ENESIM_RASTERIZER_FILL_POLICY_FILL);

	return c;
}

/* TODO instead of reseting the rasterizer and add the vertex every time
 * just add a callback for radius, x and y change and reset there
 */
static void circle_render(void *ec, void *cd, Eina_Rectangle *clip)
{
	Eon_Coord x;
	Eon_Coord y;
	int radius;
	Circle *c = ec;

	eon_circle_x_get(c->c, &x);
	eon_circle_y_get(c->c, &y);
	radius = eon_circle_radius_get(c->c);
	printf("rendering a Circle %d %d %d!!!\n", x.final, y.final, radius);
}

static void circle_delete(void *ec)
{
	Circle *c = ec;

	enesim_rasterizer_delete(c->r);
	free(ec);
}

static void _ctor(void *instance)
{
	Eon_Engine_Enesim *e;
	Eon_Engine_Enesim_Private *prv;

	e = (Eon_Engine_Enesim *) instance;
	e->prv = prv = ekeko_type_instance_private_get(eon_engine_enesim_type_get(), instance);
	e->parent.rect_create = rect_create;
	e->parent.rect_render = rect_render;
	e->parent.circle_create = circle_create;
	e->parent.circle_render = circle_render;
}

static void _dtor(void *instance)
{

}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Type * eon_engine_enesim_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_ENGINE_ENESIM, sizeof(Eon_Engine_Enesim),
				sizeof(Eon_Engine_Enesim_Private), eon_engine_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
