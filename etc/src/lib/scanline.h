#ifndef _SCANLINE_H
#define _SCANLINE_H

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
	Etc_Active_Edge *aedge;
	/* active edge table */
	int num_vertices;
	int num_edges;
	int num_allocated;
};

#endif
