#include "Enesim.h"
#include "enesim_private.h"
#include "renderer.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Fill_Color
{
	DATA32	color;
} Fill_Color;
	
void _draw(Enesim_Renderer *r, Enesim_Scanline *sl, Enesim_Surface *dst)
{
	Fill_Color *f;
	int nsl;
	int y, x, w, c;

#if 0
	f = r->data;
	nsl = sl->num;
	enesim_surface_color_func_get(dst, r->rop, sl->anti_alias);
	for (i = 0; i < nsl; i++)
	{
		int offset;
		sl->get(&y, &x, &w, &c);
		offset = (dst->w * y) + x;
		dst->color_draw(dst, offset, w, c);
	}
#endif
}

Enesim_Renderer_Func f_func;

static void _new(Enesim_Renderer *r)
{
	Fill_Color *f;

	f = calloc(1, sizeof(Fill_Color));
	r->data = f;
	r->funcs = &f_func;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Enesim_Renderer * enesim_fill_color_new(void)
{
	Enesim_Renderer *r;

	r = enesim_renderer_new();
	return r;
}

EAPI void enesim_fill_color_color_set(Enesim_Renderer *r)
{

}
