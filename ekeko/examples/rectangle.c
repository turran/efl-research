#include "Ekeko.h"
#include "ekeko_test.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _free(void *data)
{
}

static void _pre_process(void *data)
{
}

static void _process(void *data, Enesim_Rectangle *r)
{
	Object *o = data;
	SDL_Rect rect;
	
	//printf("(%d) %d %d %d %d\n", o->color, r->x, r->y, r->w, r->h);
	rect.x = r->x;
	rect.y = r->y;
	rect.w = r->w;
	rect.h = r->h;
	SDL_FillRect(o->canvas->surface, &rect, o->color);
}

static void _post_process(void *data)
{

}

static Ekeko_Object_Class _rectangle_class = {
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
	o = object_new(c, &_rectangle_class, NULL);

	return o;
}
