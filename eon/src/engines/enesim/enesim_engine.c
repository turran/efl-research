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


static void _color_set(void *c, int color)
{
	uint32_t cmul;
	uint8_t a = color >> 24;
	if (a != 256)
	{
		cmul = (color & 0xff000000) + (((((color) >> 8) & 0xff) * a) & 0xff00) +
			(((((color) & 0x00ff00ff) * a) >> 8) & 0x00ff00ff);
	}
	else
		color = cmul;
}
/*============================================================================*
 *                                  Paint                                    *
 *============================================================================*/
/* FIXME this should be the new renderer interface on enesim */
typedef struct Paint
{
	Enesim_Renderer *r;
	Eon_Paint *p;
	void *data;
} Paint;
/*============================================================================*
 *                                 Horswitch                                  *
 *============================================================================*/
void * horswitch_create(Eon_Hswitch *hs)
{
	Paint *p;

	p = calloc(1, sizeof(Paint));
	p->p = (Eon_Paint *)hs;
	p->r = enesim_renderer_hswitch_new();

	return p;
}

Eina_Bool horswitch_setup(void *data, Eon_Shape *s)
{

}

/*============================================================================*
 *                                   Image                                    *
 *============================================================================*/

/* FIXME do a real pattern! */
void _image_pattern_span(void *data, void *span, int x, int y, unsigned int len)
{
	uint32_t *dst = span;
	uint32_t color[2];
	const int size = 10;

	if ((y / size) % 2)
	{
		color[0] = 0xff555555;
		color[1] = 0x00000000;
	}
	else
	{
		color[1] = 0xff555555;
		color[0] = 0x00000000;
	}
	while (len--)
	{
		*dst = color[(x / size) % 2];
		dst++;
		x++;
	}
}
#if 0
/*
 * TODO we should use the SCALER2D
 * TODO use another scale qualities
 * render an image just scaling it
 */
static void _image_get_span_scale(void *data, void *dst, int x, int y, unsigned int len)
{
	Image *im = data;
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Operator scaler;
	uint32_t sstride;
	uint32_t *s;
	int sy;
	Eina_Rectangle r;

	eina_rectangle_coords_from(&r, x, y, len, 1);
	if (!eina_rectangle_intersection(&r, &im->drect))
		return;

	cpus = enesim_cpu_get(&numcpus);
	enesim_scaler_1d_op_get(&scaler, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);

	s = enesim_surface_data_get(im->src);
	sstride = enesim_surface_stride_get(im->src);

	//printf("SRECT = %d %d %d %d\n", im->srect.x, im->srect.y, im->srect.w, im->srect.h);
	//printf("DRECT = %d %d %d %d\n", im->drect.x, im->drect.y, im->drect.w, im->drect.h);
	//printf("%d %d %d\n", r.x, r.y, r.w);
	//printf("SGEOM %d %d %d %d\n", sgeom->x, sgeom->y, sgeom->w, sgeom->h);
	//printf("SCLIP %d %d %d %d\n", sclip.x, sclip.y, sclip.w, sclip.h);
	//printf("ENDING\n");

	/* offset the source in x */
	s = s + (((r.x - im->drect.x) * im->srect.w) / im->drect.w);
	/* pick the to-scaled line */
	sy = (((r.y - im->drect.y) * im->srect.h) / im->drect.h);
	s = s + (sy * sstride);
	enesim_operator_scaler_1d(&scaler, s, im->srect.w, 0, r.w, im->drect.w, dst);
	/* increment the source by the scale factor */
}
static void _image_get_span_noscale(void *data, void *dst, int x, int y, unsigned int len)
{
	Image *im = data;
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Operator op;
	uint32_t *s, *d;
	Eina_Rectangle r;

	eina_rectangle_coords_from(&r, x, y, len, 1);
	eina_rectangle_intersection(&r, &im->drect);

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_pixel_op_get(cpus[0], &op, ENESIM_FILL, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888);
	s = enesim_surface_data_get(im->src);
	s += (enesim_surface_stride_get(im->src) * r.y) + r.x;
	d = dst;

	printf("drawing %d %d %d\n", r.x, r.y, r.w);
	enesim_operator_drawer_span(&op, d, r.w, s, 0, NULL);
}
#endif

void * image_create(Eon_Image *i)
{
	Paint *p;

	p = calloc(1, sizeof(Paint));
	p->p = (Eon_Paint *)i;
	p->r = enesim_renderer_surface_new();

	return p;
}

Eina_Bool image_setup(void *data, Eon_Shape *s)
{
	Paint *p = data;
	Eon_Image *i = (Eon_Image *)p->p;
	Eon_Coord px, py, pw, ph;
	Eina_Rectangle geom;
	int dx, dy, dw, dh;

	if (!eon_image_loaded(i))
	{
		return EINA_FALSE;
	}
	/* setup the renderer correctly */
	if (eon_paint_coordspace_get(p->p) == EON_COORDSPACE_OBJECT)
	{
		ekeko_renderable_geometry_get((Ekeko_Renderable *)s, &geom);
	}
	else
	{
		/* FIXME we should get the topmost canvas uints not the parent
		 * canvas
		 */
		ekeko_renderable_geometry_get((Ekeko_Renderable *)eon_shape_canvas_topmost_get(s), &geom);
	}
	eon_paint_coords_get(p->p, &px, &py, &pw, &ph);
	if (px.type == EON_COORD_RELATIVE)
	{
		dx = geom.x + ((px.value * geom.w) / 100);
	}
	if (py.type == EON_COORD_RELATIVE)
	{
		dy = geom.y + ((py.value * geom.h) / 100);
	}
	if (pw.type == EON_COORD_RELATIVE)
	{
		dw = pw.value * geom.w / 100;
	}
	if (ph.type == EON_COORD_RELATIVE)
	{
		dh = ph.value * geom.h / 100;
	}
	enesim_renderer_surface_w_set(p->r, dw);
	enesim_renderer_surface_h_set(p->r, dh);
	enesim_renderer_surface_src_set(p->r, eon_image_surface_get(i));
	return EINA_TRUE;
#if 0
	im->src = eon_image_surface_get(i);
	/* check the scaling */
	im->srect.x = 0;
	im->srect.y = 0;
	eon_image_size_get(i, &im->srect.w, &im->srect.h);
	if (im->drect.w == im->srect.w && im->drect.h == im->srect.h)
		p->r.get_span = _image_get_span_noscale;
	else
		p->r.get_span = _image_get_span_scale;
	/* TODO port the transform one */
#endif
}

static void image_delete(void *i)
{
	Paint *p = i;

	free(p->data);
	free(p);
}

#if 0
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
#endif

/*============================================================================*
 *                                  Common                                    *
 *============================================================================*/
/* TODO add all the possible compositor parameters */
typedef struct Shape_Drawer_Data
{
	Enesim_Surface *dst;
	Enesim_Operator op;
	Eon_Color color;
	/* the callback */
	Enesim_Scanline_Callback cb;
	/* paint engine data */
	Paint *paint;
} Shape_Drawer_Data;


void aliased_color_cb(Enesim_Scanline *sl, void *data)
{
	Shape_Drawer_Data *sdd = data;
	uint32_t *ddata;
	uint32_t stride;

	ddata = enesim_surface_data_get(sdd->dst);
	stride = enesim_surface_stride_get(sdd->dst);
	ddata = ddata + (sl->data.alias.y * stride) + sl->data.alias.x;
	enesim_operator_drawer_span(&sdd->op, ddata, sl->data.alias.w, NULL, sdd->color, NULL);
}

void aliased_fill_cb(Enesim_Scanline *sl, void *data)
{
	Shape_Drawer_Data *sdd = data;
	uint32_t *ddata;
	uint32_t stride;
	uint32_t *fdata;

	ddata = enesim_surface_data_get(sdd->dst);
	stride = enesim_surface_stride_get(sdd->dst);
	ddata = ddata + (sl->data.alias.y * stride) + sl->data.alias.x;

	/* fill the new span */
	fdata = calloc(sl->data.alias.w, sizeof(uint32_t));
	/* TODO match the coordinates */
	/* clip the paint coordinates to the shape's ones, only pass those */
	enesim_renderer_span_fill(sdd->paint->r, sl->data.alias.x, sl->data.alias.y, sl->data.alias.w, fdata);

	/* compose the filled and the destination spans */
	enesim_operator_drawer_span(&sdd->op, ddata, sl->data.alias.w, fdata, sdd->color, NULL);
	free(fdata);
}

static void shape_setup(Eon_Shape *s, Shape_Drawer_Data *d, Enesim_Surface *dst)
{
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Rop rop;
	Eon_Paint *p;

	cpus = enesim_cpu_get(&numcpus);
	rop = eon_shape_rop_get(s);
	d->color = eon_shape_color_get(s);
	d->dst = dst;
	p = eon_shape_fill_get(s);
	/* color based */
	if (!p)
	{
		enesim_drawer_span_color_op_get(cpus[0], &d->op, rop, ENESIM_FORMAT_ARGB8888, d->color);
		d->cb = aliased_color_cb;
	}
	/* paint based */
	else
	{
		/* TODO paint + color */
		d->paint = eon_paint_engine_data_get(p);
		enesim_drawer_span_pixel_color_op_get(cpus[0], &d->op, rop, ENESIM_FORMAT_ARGB8888, ENESIM_FORMAT_ARGB8888, d->color);
		d->cb = aliased_fill_cb;
	}
}

/*============================================================================*
 *                                  Polygon                                   *
 *============================================================================*/
typedef struct Polygon
{
	Eon_Polygon *p;
	Enesim_Rasterizer *r;
} Polygon;

static void * polygon_create(Eon_Polygon *ep)
{
	Polygon *p = calloc(1, sizeof(Polygon));
	p->p = ep;
	/* FIXME alias by now */
	p->r = enesim_rasterizer_cpsc_new();

	return p;
}

void polygon_point_add(void *pd, int x, int y)
{
	Polygon *p = pd;

	/* create a rasterizer */
	/* TODO check the quality to create a kiia or a cpsc rasterizer */
	/* add a vertex to it */
	enesim_rasterizer_vertex_add(p->r, x, y);
}

void polygon_render(void *pd, void *cd, Eina_Rectangle *clip)
{
	Polygon *p = pd;
	Shape_Drawer_Data sdd;

	shape_setup((Eon_Shape *)p->p, &sdd, cd);
	enesim_rasterizer_generate(p->r, clip, sdd.cb, &sdd);
}

static void polygon_delete(void *ep)
{
	Polygon *p = ep;

	enesim_rasterizer_delete(p->r);
	free(p);
}
/*============================================================================*
 *                                   Rect                                     *
 *============================================================================*/
static void * rect_create(Eon_Rect *r)
{
	return r;
}

static void rect_render(void *er, void *cd, Eina_Rectangle *clip)
{

	Enesim_Scanline sl;
	Eon_Rect *r = er;
	Shape_Drawer_Data sdd;
	uint32_t *dst;
	uint32_t stride;

	shape_setup((Eon_Shape *)r, &sdd, cd);
	stride = enesim_surface_stride_get(sdd.dst);
	sl.type = ENESIM_SCANLINE_ALIAS;
	sl.data.alias.x = clip->x;
	sl.data.alias.y = clip->y;
	sl.data.alias.w = clip->w;

	/* we are using the same functions as the other rasterizers
	 * but the surface pointer calculations has to be done on every callback
	 */
	while (clip->h--)
	{
		sdd.cb(&sl, &sdd);
		sl.data.alias.y++;
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
	Circle *c = ec;
	Eon_Coord x;
	Eon_Coord y;
	int radius;
	Shape_Drawer_Data sdd;

	eon_circle_x_get(c->c, &x);
	eon_circle_y_get(c->c, &y);
	radius = eon_circle_radius_get(c->c);
	enesim_rasterizer_circle_radius_set(c->r, radius);
        enesim_rasterizer_vertex_add(c->r, x.final, y.final);
	shape_setup((Eon_Shape *)c->c, &sdd, cd);
	enesim_rasterizer_generate(c->r, clip, sdd.cb, &sdd);
}

static void circle_delete(void *ec)
{
	Circle *c = ec;

	enesim_rasterizer_delete(c->r);
	free(ec);
}
/*============================================================================*
 *                                  Debug                                     *
 *============================================================================*/
static void debug_rect(void *cd, uint32_t color, int x, int y, int w, int h)
{
	Enesim_Cpu **cpus;
	int numcpus;
	Enesim_Surface *s = cd;
	uint32_t *dst;
	uint32_t stride;
	Enesim_Operator op;

	cpus = enesim_cpu_get(&numcpus);
	enesim_drawer_span_color_op_get(cpus[0], &op, ENESIM_FILL, ENESIM_FORMAT_ARGB8888, color);

	dst = enesim_surface_data_get(s);
	stride = enesim_surface_stride_get(s);
	dst = dst + (y * stride) + x;

	while (h--)
	{
		enesim_operator_drawer_span(&op, dst, w, NULL, color, NULL);
		dst += stride;
	}
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
	e->parent.polygon_create = polygon_create;
	e->parent.polygon_point_add = polygon_point_add;
	e->parent.polygon_render = polygon_render;
	e->parent.image_create = image_create;
	e->parent.image_delete = image_delete;
	e->parent.image_setup = image_setup;
	e->parent.debug_rect = debug_rect;
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
