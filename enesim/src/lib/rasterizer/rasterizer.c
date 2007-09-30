#include <stdlib.h>

#include "Etc.h"
#include "etc_private.h"
#include "rasterizer.h"
#include "scanline.h"

/**
 * TODO
 * add a function/struct element to set up window boundaries i.e 
 * destination surface/clip
 * add a function/struct element to set up the odd/even/zero fill rule
 */

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Etc_Rasterizer * etc_rasterizer_new(void)
{
	Etc_Rasterizer *r;

	r = calloc(1, sizeof(Etc_Rasterizer));
	r->funcs = &cpsc;
	r->data = cpsc.create();
	return r;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etc_rasterizer_vertex_add(Etc_Rasterizer *r, float x, float y)
{
	r->funcs->vertex_add(r->data, x, y);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etc_rasterizer_generate(Etc_Rasterizer *r, Etc_Scanline *s)
{
	r->funcs->generate(r->data, s);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etc_rasterizer_boundings_set(Etc_Rasterizer *r, int x, int y, int w, int h)
{

}
