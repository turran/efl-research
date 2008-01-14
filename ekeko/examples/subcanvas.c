#include "Ekeko.h"
#include "ekeko_test.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* A subcanvas implements both interfaces: Object and Canvas,
 * so the object_ canvas_ functions are still available
 */
struct _Subcanvas
{
	Object *object;
	Canvas *canvas;
};

static void _flush(void *data, Ekeko_Rectangle *rects)
{
	Subcanvas *s = data;
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)rects; l; l = l->next)
	{
		Ekeko_Rectangle *r;
		SDL_Rect rect;

		r = (Ekeko_Rectangle *)l;
		rect.x = r->r.x;
		rect.y = r->r.y;
		rect.w = r->r.w;
		rect.h = r->r.h;

		/* transform this rectangle relative to the upper canvas
		 * and blit between the two surfaces */
		SDL_BlitSurface(s->canvas->surface, NULL, s->object->canvas->surface, &rect);
		printf("SUBCANVAS FLUSH %p\n", s->canvas->surface);
	}
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

static void _process(void *data, Enesim_Rectangle *r)
{
	Subcanvas *s = data;
	/* the rectangle received is relative to the upper canvas
	 * transform it to the local coordinates */
	/* pass this rectangle as a damage */
	printf("processing subcanvas\n");
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
