#include <stdlib.h>

#include "Etc.h"
#include "etc_private.h"
#include "scanline.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

#define ALLOC_STEP 1024

static inline etc_scanline_edge_new(Etc_Scanline *s)
{
	if (s->num_edges == s->num_allocated)
	{
		s->edges = realloc(s->edges, sizeof(Etc_Edge) * (s->num_edges + ALLOC_STEP));
		s->num_allocated += ALLOC_STEP;
	}
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Etc_Scanline * etc_scanline_new(void)
{
	Etc_Scanline *s;

	s = calloc(1, sizeof(Etc_Scanline));
	return s;
}

EAPI void etc_scanline_edge_reset(Etc_Scanline *s)
{
	s->num_vertices = 0;
	s->num_edges = 0;
}


EAPI void etc_scanline_edge_add(Etc_Scanline *s, float x0, float y0, float x1, float y1)
{
	etc_scanline_edge_new(s);
	s->edges[s->num_edges].x0 = x0;
	s->edges[s->num_edges].y0 = y0;
	s->edges[s->num_edges].x1 = x1;
	s->edges[s->num_edges].y1 = y1;
	s->num_edges++;
	s->num_vertices += 2;
}

EAPI void etc_scanline_vertex_add(Etc_Scanline *s, float x, float y)
{
	if (!(s->num_vertices % 2))
	{
		etc_scanline_edge_new(s);
		s->edges[s->num_edges].x0 = x;
		s->edges[s->num_edges].y0 = y;
		s->num_vertices++;
	}
	else
	{
		s->edges[s->num_edges].x1 = x;
		s->edges[s->num_edges].y1 = y;
		s->num_vertices++;
		s->num_edges++;
	}
}

EAPI void etc_scanline_generate(Etc_Scanline *s)
{
	int i;
	/* generate the active edge table */
	for (i = 0; i < s->num_edges; i++)
	{
		printf("edge %d, %fx%f => %fx%f\n", i, s->edges[i].x0, s->edges[i].y0, s->edges[i].x1, s->edges[i].y1);
	}
}
