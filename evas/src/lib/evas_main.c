#include "Evas.h"
#include "evas_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _flush(void *data, Enesim_Rectangle *r)
{
	printf("called\n");
	/* call the engine function */
	return 1;
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
static int _initcount = 0;

EAPI int
evas_init(void)
{
	if (_initcount == 0)
	evas_module_init();
	return ++_initcount;
}

EAPI int
evas_shutdown(void)
{
	_initcount--;
	if (_initcount == 0)
	{
		evas_module_shutdown();
	}
	return _initcount;
}

EAPI Evas *
evas_new(void)
{
	Evas *e;

	e = calloc(1, sizeof(Evas));
	if (!e) return NULL;
#ifdef DEBUG
	e->magic = MAGIC_EVAS;
#endif
	return e;
}


EAPI void
evas_free(Evas *e)
{
	assert(e);
	free(e);
}

EAPI void
evas_size_set(Evas *e, int w, int h)
{
	assert(e);
	assert(w);
	assert(h);
	
	if ((!e->w) && (!e->h))
	{
		e->canvas = ekeko_canvas_new(&_canvas_class, e, EKEKO_TILER_SPLITTER, w, h);
		
	}
	else
	{
		/* resize the ekeko canvas */
	}
	e->w = w;
	e->h = h;
}

EAPI void
evas_size_get(const Evas *e, int *w, int *h)
{
	assert(e);
	
	if (w) *w = e->w;
	if (h) *h = e->h;
}

EAPI void
evas_output_set(Evas *e, const char *engine, const char *depth, const char *options)
{
	assert(e);
	/* sdl
	 * ENESIM_ARGB_565?
	 * fullsreen=xxx,noframe,etc
	 */
	/* comma separated options */
	/* every option can be */
}


EAPI void
evas_render(Evas *e)
{
	assert(e);
	assert(e->canvas);
#if 0
	MAGIC_CHECK(e, Evas, MAGIC_EVAS);
	return;
	MAGIC_CHECK_END();
#endif
	/* TODO move this to ekeko *
	if (!e->changed) return; */
	ekeko_canvas_process(e->canvas);
}
