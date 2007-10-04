#include "Enesim.h"
#include "enesim_private.h"
#include "renderer.h"
#include "surface.h"
#include "scanline.h"
#include "alias.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Fill_Surface
{
	Enesim_Surface 	s;
	int 		mode;
	//Enesim_Rectangle area;
} Fill_Surface;

void _draw_alias(Enesim_Renderer *r, Scanline_Alias *sl, Enesim_Surface *dst)
{
}

void _draw(Enesim_Renderer *r, Enesim_Scanline *sl, Enesim_Surface *dst)
{
	_draw_alias(r, sl->data, dst);
}

void _free(Enesim_Renderer *r)
{
	free(r->data);
}

Enesim_Renderer_Func f_func = {
	.draw 	= _draw,
	.free 	= _free,
};
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * enesim_fill_surface_new(void)
{
	Fill_Surface *f;
	Enesim_Renderer *r;

	f = calloc(1, sizeof(Fill_Surface));
	
	r = enesim_renderer_new();
	r->data = f;
	r->funcs = &f_func;

	return r;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_fill_surface_surface_set(Enesim_Renderer *r, Enesim_Surface *s)
{
	Fill_Surface *f;

	assert(r);
	f = r->data;
	f->surface = s;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_fill_surface_mode_set(Enesim_Renderer *r, int mode)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_fill_surface_area_set(Enesim_Renderer *r, int x, int y, int w, int h)
{

}
