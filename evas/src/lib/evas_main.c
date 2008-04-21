#include "Evas.h"
#include "evas_private.h"
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

/**
 * Creates a new empty evas.
 *
 * Note that before you can use the evas, you will to at a minimum:
 * @li Set its render method with @ref evas_output_method_set .
 * @li Set its viewport size with @ref evas_output_viewport_set .
 * @li Set its size of the canvas with @ref evas_output_size_set .
 * @li Ensure that the render engine is given the correct settings with
 *     @ref evas_engine_info_set .
 *
 * This function should only fail if the memory allocation fails.
 *
 * @return  A new uninitialised Evas canvas on success.  Otherwise, @c NULL.
 * @ingroup Evas_Canvas
 */
EAPI Evas *
evas_new(void)
{
	Evas *e;

	e = calloc(1, sizeof(Evas));
	if (!e) return NULL;
#ifdef DEBUG
	e->magic = MAGIC_EVAS;
#endif
#if 0
	e->viewport.w = 1;
	e->viewport.h = 1;
	e->hinting = EVAS_FONT_HINTING_BYTECODE;
#endif
	return e;
}


EAPI void evas_free(Evas *e)
{
	
}
EAPI void evas_output_size_set(Evas *e, int w, int h)
{
	
}
EAPI void evas_output_size_get(const Evas *e, int *w, int *h)
{
	
}
EAPI void evas_output_viewport_set(Evas *e, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h)
{
	
}
EAPI void evas_output_viewport_get(const Evas *e, Evas_Coord *x, Evas_Coord *y, Evas_Coord *w, Evas_Coord *h)
{
	
}
