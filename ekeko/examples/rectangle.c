#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _create(Ekeko_Object *o)
{
#if 0
	Rectangle *rect;

	rect = malloc(sizeof(Rectangle));
	rect->object = o;
	ekeko_object_data_set(o, rect);
#endif
}

static void _free(void *data)
{
#if 0
	free(data);
#endif
}

static void _pre_process(void *data)
{
	//printf("pre\n");
}

static void _process(void *data, Enesim_Rectangle *r)
{
	Object *o;
	
	Ekeko_Canvas *canvas;
		
	SDL_Surface *surface;
	SDL_Rect rect;
	
	o = data;
	canvas = o->canvas->canvas;
	surface = ekeko_canvas_data_get(o->canvas->canvas);
	//printf("(%d) %d %d %d %d\n", o->color, r->x, r->y, r->w, r->h);
	rect.x = r->x;
	rect.y = r->y;
	rect.w = r->w;
	rect.h = r->h;
	SDL_FillRect(surface, &rect, o->color);
	// FIXME for now, update the rect like this, it should be on the canvas callback */
	SDL_UpdateRect(surface, rect.x, rect.y, rect.w, rect.h);
}

static void _post_process(void *data)
{

}

Ekeko_Object_Class rectangle_class = {
	.create = _create,
	.free = _free,
	.pre_process = _pre_process,
	.process = _process,
	.post_process = _post_process,
};


/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Object * rectangle_new(Canvas *c)
{
	Object *o;
	
	o = object_new(c, &rectangle_class, NULL);
	return o;
}
