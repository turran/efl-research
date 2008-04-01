#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Emage.h"
#include "emage_private.h"

struct _Emage_Polygon_Point
{
   Evas_Object_List  _list_data;
   int               x, y;
};

typedef struct _RGBA_Edge RGBA_Edge;
typedef struct _RGBA_Vertex RGBA_Vertex;

struct _RGBA_Edge
{
   double x, dx;
   int i;
};

struct _RGBA_Vertex
{
   double x, y;
   int i;
};

/*============================================================================*
 *                                 Macros                                     * 
 *============================================================================*/

#define POLY_EDGE_DEL(_i)                                               \
{                                                                       \
   int _j;                                                              \
                                                                        \
   for (_j = 0; (_j < num_active_edges) && (edges[_j].i != _i); _j++);  \
   if (_j < num_active_edges)                                           \
     {                                                                  \
	num_active_edges--;                                             \
	memmove(&(edges[_j]), &(edges[_j + 1]),                         \
	        (num_active_edges - _j) * sizeof(RGBA_Edge));           \
     }                                                                  \
}

#define POLY_EDGE_ADD(_i, _y)                                           \
{                                                                       \
   int _j;                                                              \
   float _dx;                                                           \
   RGBA_Vertex *_p, *_q;                                                \
   if (_i < (n - 1)) _j = _i + 1;                                       \
   else _j = 0;                                                         \
   if (point[_i].y < point[_j].y)                                       \
     {                                                                  \
	_p = &(point[_i]);                                              \
	_q = &(point[_j]);                                              \
     }                                                                  \
   else                                                                 \
     {                                                                  \
	_p = &(point[_j]);                                              \
	_q = &(point[_i]);                                              \
     }                                                                  \
   edges[num_active_edges].dx = _dx = (_q->x - _p->x) / (_q->y - _p->y); \
   edges[num_active_edges].x = (_dx * ((float)_y + 0.5 - _p->y)) + _p->x; \
   edges[num_active_edges].i = _i;                                      \
   num_active_edges++;                                                  \
}

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

static int
polygon_point_sorter(const void *a, const void *b)
{
   RGBA_Vertex *p, *q;

   p = (RGBA_Vertex *)a;
   q = (RGBA_Vertex *)b;
   if (p->y <= q->y) return -1;
   return 1;
}

static int
polygon_edge_sorter(const void *a, const void *b)
{
   RGBA_Edge *p, *q;

   p = (RGBA_Edge *)a;
   q = (RGBA_Edge *)b;
   if (p->x <= q->x) return -1;
   return 1;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * TODO
 */
EAPI Emage_Polygon_Point *
emage_polygon_point_add(Emage_Polygon_Point *points, int x, int y)
{
   Emage_Polygon_Point *pt;

   pt = malloc(sizeof(Emage_Polygon_Point));
   if (!pt) return points;
   pt->x = x;
   pt->y = y;
   points = evas_object_list_append(points, pt);
   return points;
}

/**
 * TODO
 */
EAPI Emage_Polygon_Point *
emage_polygon_points_clear(Emage_Polygon_Point *points)
{
   if (points)
     {
	while (points)
	  {
	     Emage_Polygon_Point *old_p;

	     old_p = points;
	     points = evas_object_list_remove(points, points);
	     free(old_p);
	  }
     }
   return NULL;
}

/**
 * TODO
 */
EAPI void
emage_polygon_draw(Emage_Surface *dst, Emage_Draw_Context *dc, Emage_Polygon_Point *points)
{
   Emage_Polygon_Point *pt;
   RGBA_Vertex       *point;
   RGBA_Edge         *edges;
   Evas_Object_List  *spans, *l;
   int                num_active_edges;
   int                n;
   int                i, j, k;
   int                y0, y1, y;
   int                ext_x, ext_y, ext_w, ext_h;
   int               *sorted_index;

   ext_x = 0;
   ext_y = 0;
   ext_w = dst->w;
   ext_h = dst->h;
   if (dc->clip.use)
     {
	if (dc->clip.r.x > ext_x)
	  {
	     ext_w += ext_x - dc->clip.r.x;
	     ext_x = dc->clip.r.x;
	  }
	if ((ext_x + ext_w) > (dc->clip.r.x + dc->clip.r.w))
	  {
	     ext_w = (dc->clip.r.x + dc->clip.r.w) - ext_x;
	  }
	if (dc->clip.r.y > ext_y)
	  {
	     ext_h += ext_y - dc->clip.r.y;
	     ext_y = dc->clip.r.y;
	  }
	if ((ext_y + ext_h) > (dc->clip.r.y + dc->clip.r.h))
	  {
	     ext_h = (dc->clip.r.y + dc->clip.r.h) - ext_y;
	  }
     }
   if ((ext_w <= 0) || (ext_h <= 0)) return;

   evas_common_cpu_end_opt();

   n = 0; for (l = (Evas_Object_List *)points; l; l = l->next) n++;
   if (n < 3) return;
   edges = malloc(sizeof(RGBA_Edge) * n);
   if (!edges) return;
   point = malloc(sizeof(RGBA_Vertex) * n);
   if (!point)
     {
	free(edges);
	return;
     }
   sorted_index = malloc(sizeof(int) * n);
   if (!sorted_index)
     {
	free(edges);
	free(point);
	return;
     }

   k = 0;
   for (l = (Evas_Object_List *)points; l; l = l->next)
     {
	pt = (Emage_Polygon_Point *)l;
	point[k].x = pt->x;
	point[k].y = pt->y;
	point[k].i = k;
	k++;
     }
   qsort(point, n, sizeof(RGBA_Vertex), polygon_point_sorter);
   for (k = 0; k < n; k++) sorted_index[k] = point[k].i;
   k = 0;
   for (l = (Evas_Object_List *)points; l; l = l->next)
     {
	pt = (Emage_Polygon_Point *)l;
	point[k].x = pt->x;
	point[k].y = pt->y;
	point[k].i = k;
	k++;
     }

   y0 = MAX(ext_y, ceil(point[sorted_index[0]].y - 0.5));
   y1 = MIN(ext_y + ext_h - 1, floor(point[sorted_index[n - 1]].y - 0.5));

   k = 0;
   num_active_edges = 0;
   spans = NULL;

   for (y = y0; y <= y1; y++)
     {
	for (; (k < n) && (point[sorted_index[k]].y <= ((double)y + 0.5)); k++)
	  {
	     i = sorted_index[k];

	     if (i > 0) j = i - 1;
	     else j = n - 1;
	     if (point[j].y <= ((double)y - 0.5))
	       {
		  POLY_EDGE_DEL(j)
	       }
	     else if (point[j].y > ((double)y + 0.5))
	       {
		  POLY_EDGE_ADD(j, y)
	       }
	     if (i < (n - 1)) j = i + 1;
	     else j = 0;
	     if (point[j].y <= ((double)y - 0.5))
	       {
		  POLY_EDGE_DEL(i)
	       }
	     else if (point[j].y > ((double)y + 0.5))
	       {
		  POLY_EDGE_ADD(i, y)
	       }
	  }

	qsort(edges, num_active_edges, sizeof(RGBA_Edge), polygon_edge_sorter);

	for (j = 0; j < num_active_edges; j += 2)
	  {
	     int x0, x1;

	     x0 = ceil(edges[j].x - 0.5);
	     if (j < (num_active_edges - 1))
	       x1 = floor(edges[j + 1].x - 0.5);
	     else
	       x1 = x0;
	     if ((x1 >= ext_x) && (x0 < (ext_x + ext_w)) && (x0 <= x1))
	       {
	
		  Emage_Scanline sl;

		  if (x0 < ext_x) x0 = ext_x;
		  if (x1 >= (ext_x + ext_w)) x1 = ext_x + ext_w - 1;
		  
		  sl.y = y;
		  sl.x = x0;
		  sl.w = (x1 - x0) + 1;
		  sl.coverage = 32; // FIXME
		  emage_scanline_draw(&sl, dst, dc);
	       }
	     edges[j].x += edges[j].dx;
	     edges[j + 1].x += edges[j + 1].dx;
	  }
     }

   free(edges);
   free(point);
   free(sorted_index);
}
