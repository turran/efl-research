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

static void _process(void *data, Eina_Rectangle *r)
{
	Object *o = data;
	unsigned int *p1, *eh, *ew;
	
	/* simple filter, invert the colors
	 * we should use the pitch instead of the width, but works for now, this is only a test :)
	 */
	SDL_LockSurface(o->canvas->surface);
	p1 = (unsigned int *)o->canvas->surface->pixels + (r->y * o->canvas->surface->w) + r->x;
	eh = p1 + (r->h * o->canvas->surface->w) + r->w;
	while (p1 < eh)
	{
		unsigned int *p2, *ew;
		
		p2 = p1;
		ew = p2 + r->w;
		
		while (p2 < ew)
		{
			*p2 = ~*p2;
			p2++;
		}
		p1 += o->canvas->surface->w;
	}
	SDL_UnlockSurface(o->canvas->surface);
}

static void _post_process(void *data)
{

}

static Ekeko_Object_Class _filter_class = {
	.name = "filter",
	.free = _free,
	.pre_process = _pre_process,
	.process = _process,
	.post_process = _post_process,
};


/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Object * filter_new(Canvas *c)
{
	Object *o;
	o = object_new(c, &_filter_class, NULL);

	return o;
}
