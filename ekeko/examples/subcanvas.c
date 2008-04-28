#include "Ekeko.h"
#include "ekeko_test.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* A subcanvas implements both interfaces: Object and Canvas,
 * so the object_ canvas_ functions are still available.
 * TODO We should do it in the inverse way, the flush should notify damage rectangles
 * and the process_whatever should blit 
 */
struct _Subcanvas
{
	Object *object;
	Canvas *canvas;
};

static int _flush(void *data, Eina_Rectangle *r)
{
	Subcanvas *s = data;
	Eina_Rectangle geometry;
	Eina_Rectangle rscaled;

	/* transform this rectangle relative to the upper canvas */ 
	ekeko_object_geometry_get(s->object->object, &geometry);
	eina_rectangle_rescale_out(&geometry, r, &rscaled);
	/* and mark those rectangles as a damage */
	ekeko_canvas_damage_add(s->object->canvas->canvas, &rscaled);
	return 1;
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};

static void _free(void *data)
{
}

static void _pre_process(void *data)
{
}

static void _process(void *data, Eina_Rectangle *r)
{
	Subcanvas *s = data;
	Eina_Rectangle geometry;
	Eina_Rectangle rscaled;
	SDL_Rect srect;
	SDL_Rect drect;
	
	/* the rectangle received is relative to the upper canvas
	 * transform it to the local coordinates */
	ekeko_object_geometry_get(s->object->object, &geometry);
	eina_rectangle_rescale_in(&geometry, r, &rscaled);
	/* blit the rectangle on the upper canvas */
	srect.x = rscaled.x;
	srect.y = rscaled.y;
	srect.w = rscaled.w;
	srect.h = rscaled.h;
	
	drect.x = r->x;
	drect.y = r->y;
	drect.w = r->w;
	drect.h = r->h;
	
	//printf("SRC = ");
	//rectangle_print(&rscaled);
	//printf("DST = ");
	//rectangle_print(r);
	SDL_BlitSurface(s->canvas->surface, &srect, s->object->canvas->surface, &drect);
}

static void _post_process(void *data)
{

}

static Ekeko_Object_Class _object_class = {
	.free = _free,
	.pre_process = _pre_process,
	.process = _process,
	.post_process = _post_process,
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Subcanvas * subcanvas_new(Canvas *c, int x, int y, int w, int h)
{
	Ekeko_Object *eo;
	Object *o;
	
	Subcanvas *s;
	SDL_Surface *surface;
	
	s = calloc(1, sizeof(Subcanvas));
	/* canvas interface */
	s->canvas = calloc(1, sizeof(Canvas));
	s->canvas->surface = SDL_CreateRGBSurface(SDL_SRCALPHA, w, h, 32,
                                0xff << 24, 0xff << 16, 0xff << 8, 0xff);
	s->canvas->canvas = ekeko_canvas_new(&_canvas_class, s, EKEKO_TILER_SPLITTER, w, h);
	/* object interface */
	s->object = object_new(c, &_object_class, s);
	ekeko_object_move(s->object->object, x, y);
	ekeko_object_resize(s->object->object, w, h);

	return s;
}

Canvas * subcanvas_canvas_get(Subcanvas *s)
{
	return s->canvas;
}

Object * subcanvas_object_get(Subcanvas *s)
{
	return s->object;
}
