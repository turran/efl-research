#include "Etc.h"
#include "etc_private.h"
#include "scanline.h"

typedef struct _Faap_Vertex
{

} Faap_Vertex;


#define SUBYRES 8
#define SUBXRES 16

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _new(Etc_Scanline *s)
{

}

static void _vertex_add(Etc_Scanline *s, float x, float y)
{
	/* transform to subpixel coordinates */
}

static void _generate(Etc_Scanline *s)
{
	/* find vertex with minimum y (display coordinate) */
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Etc_Rasterizer faap = {
	.vertex_add 	= _vertex_add,
	.generate 	= _generate
};
