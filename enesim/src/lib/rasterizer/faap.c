#include "Enesim.h"
#include "enesim_private.h"
#include "rasterizer.h"

typedef struct _Faap_Vertex
{
	int x;
	int y;
} Faap_Vertex;

typedef struct _Faap_Rasterizer
{
	Faap_Vertex 	*vertices;
	int 		num_vertices;
} Faap_Rasterizer;


#define SUBYRES 8
#define SUBXRES 16

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void * _create(void)
{

}

static void _vertex_add(Faap_Rasterizer *r, float x, float y)
{
	/* transform to subpixel coordinates */
}

static void _generate(Faap_Rasterizer *r)
{
	Faap_Vertex *vleft, *vnleft, *vright, *vnright;
	int i, y;

	/* find vertex with minimum y (display coordinate) */
	vleft = r->vertices;
	for (i = 0; i < r->num_vertices; i++)
	{
		if (r->vertices[i].y < vleft->y)
			vleft = &r->vertices[i];
	}
	vright = vnright = vnleft = vleft;
	/* initial scanline */

	/* from bottom to top */
	for (y = vleft->y; ; y++)
	{


	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Enesim_Rasterizer_Func faap = {
	.create 	= _create,
	.vertex_add 	= _vertex_add,
	.generate 	= _generate
};
