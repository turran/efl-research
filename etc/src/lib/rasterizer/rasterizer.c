#include <stdlib.h>

#include "Etc.h"
#include "etc_private.h"
#include "rasterizer.h"
#include "scanline.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Etc_Rasterizer * etc_rasterizer_new(void)
{
	Etc_Rasterizer *r;

	r = calloc(1, sizeof(Etc_Rasterizer));
	r->funcs = &gcp;
	r->data = gcp.create();
	return r;
}


EAPI void etc_rasterizer_vertex_add(Etc_Rasterizer *r, float x, float y)
{
	r->funcs->vertex_add(r->data, x, y);
}

EAPI void etc_rasterizer_generate(Etc_Rasterizer *r, Etc_Scanline *s)
{
	r->funcs->generate(r->data, s);
}

#if 0
/**
 * TODO
 * This will be just the interface, let the rasterizers implement this
 * functions
 *
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define ALLOC_STEP 1024

static inline void _edge_new(Etc_Scanline *s)
{
	if (s->num_edges == s->num_allocated)
	{
		s->edges = realloc(s->edges, sizeof(Etc_Edge) * (s->num_edges + ALLOC_STEP));
		s->num_allocated += ALLOC_STEP;
	}
}

static inline void _aedge_new(Etc_Scanline *s)
{
	if (s->num_aedges == s->num_aallocated)
	{
		s->aedges = realloc(s->aedges, sizeof(Etc_Active_Edge) * (s->num_aedges + ALLOC_STEP));
		s->num_aallocated += ALLOC_STEP;
	}
}

static inline void _scanline_generate(Etc_Scanline *s, int y)
{
	int i;

	/* 1. generate the active edge table */
	for (i = 0; i < s->num_edges; i++)
	{
		Etc_Edge *e;
		
		Etc_Active_Edge *ae;
		
		_aedge_new(s);
		ae = &(s->aedges[s->num_aedges]);
		e = &(s->edges[i]);

		ae->x0 = e->x0;
		ae->y0 = e->y0;
		ae->y1 = e->y1;
		ae->dx = e->x1 - e->x0;
		ae->dy = e->y1 - e->y0;
		/* FIXME handle lower/upper cases (direction) */
		ae->direction = ETC_EDGE_UP;
		ae->minx = 0;
		ae->maxx = 0;
	}
	/* 2. sort them by minx */
	for (i = 0; i < s->num_edges; i++)
	{
		printf("edge %d, %fx%f => %fx%f\n", i, s->edges[i].x0, s->edges[i].y0, s->edges[i].x1, s->edges[i].y1);
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/



EAPI void etc_rasterizer_edge_reset(Etc_Rasterizer *s)
{
	s->num_vertices = 0;
	s->num_edges = 0;
}


EAPI void etc_rasterizer_edge_add(Etc_Rasterizer *s, float x0, float y0, float x1, float y1)
{
	_edge_new(s);
	s->edges[s->num_edges].x0 = x0;
	s->edges[s->num_edges].y0 = y0;
	s->edges[s->num_edges].x1 = x1;
	s->edges[s->num_edges].y1 = y1;
	s->num_edges++;
	s->num_vertices += 2;
}
EAPI void etc_rasterizer_vertex_add(Etc_Rasterizer *s, float x, float y)
{
	if (!(s->num_vertices % 2))
	{
		_edge_new(s);
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
#endif
