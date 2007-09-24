#ifndef _SCANLINE_H
#define _SCANLINE_H

enum
{
	ETC_EDGE_UP,
	ETC_EDGE_DOWN,
	ETC_EDGE_DIRECTIONS
};

typedef struct _Etc_Vertex
{
	float 	x;
	float 	y;
} Etc_Vertex;

typedef struct _Etc_Edge
{
	float 	x0;
	float 	y0;
	float 	x1;
	float 	y1;
} Etc_Edge;

typedef struct _Etc_Active_Edge
{
	float 	x0;
	float 	y0;
	float 	y1;
	float 	minx;
	float 	maxx;
	float 	dx;
	float 	dy;
	int 	direction;
} Etc_Active_Edge;

struct _Etc_Scanline
{
	Etc_Edge *edges;
	int num_vertices;
	int num_edges;
	int num_allocated;
	
	Etc_Active_Edge *aedges;
	int num_aedges;
	int num_aallocated;
	void *data;
};



#if 0
/* same scheme we use on equis, better abstract them? */
struct _Etc_Alloc
{
	void 	*data;
	int 	selement;
	int 	nelements;
	int 	nalloc;
};
#endif


#endif
