#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void rectangle_class_create(Ekeko_Object *o)
{
#if 0
	Rectangle *rect;

	rect = malloc(sizeof(Rectangle));
	rect->object = o;
	ekeko_object_data_set(o, rect);
#endif
}

static void rectangle_class_free(void *data)
{
#if 0
	free(data);
#endif
}

static void rectangle_class_pre_process(void *data)
{
	//printf("pre\n");
}

static void rectangle_class_process(void *data, Enesim_Rectangle *r)
{
#if 0
	Rectangle *rectangle;
	Ekeko_Canvas *canvas;
	SDL_Rect rect;
	SDL_Surface *surface;

	rectangle = data;
	canvas = ekeko_object_canvas_get(rectangle->object);
	surface = ekeko_canvas_data_get(canvas);
	//printf("(%d) %d %d %d %d\n", rectangle->color, r->x, r->y, r->w, r->h);
	rect.x = r->x;
	rect.y = r->y;
	rect.w = r->w;
	rect.h = r->h;
	SDL_FillRect(surface, &rect, rectangle->color);
	// FIXME for now, update the rect like this, it should be on the canvas callback */
	SDL_UpdateRect(surface, rect.x, rect.y, rect.w, rect.h);
#endif
}

static void rectangle_class_post_process(void *data)
{

}

Ekeko_Object_Class rectangle_class = {
	.create = rectangle_class_create,
	.free = rectangle_class_free,
	.pre_process = rectangle_class_pre_process,
	.process = rectangle_class_process,
	.post_process = rectangle_class_post_process,
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
