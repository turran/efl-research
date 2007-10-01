#ifndef _PATH_H
#define _PATH_H

typedef struct _Enginy_Path 	Enginy_Path; /**< */
typedef struct _Enginy_Point 	Enginy_Point; /**< */

/**
 *
 */
struct _Enginy_Point
{
	float x;
	float y;
};

/**
 *
 */
struct _Enginy_Path
{
	Edata_Array	*a;
	Enginy_Point 	*points;
	char 		*cmds;
	Enginy_Point 	*point_curr;
	char 		*cmd_curr;
	int 		num_vertices;

	void 		*data;
	void		(*alloc_cb)(void *data);
};

Enginy_Path * 	enginy_path_new(void *data, int vertices_ref);
void * 		enginy_path_delete(Enginy_Path *p);
void 		enginy_path_vertex_add(Enginy_Path *p, float x, float y, char cmd);

#endif
