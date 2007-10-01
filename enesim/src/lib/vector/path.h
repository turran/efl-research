#ifndef _PATH_H
#define _PATH_H

typedef struct _Enesim_Path 	Enesim_Path; /**< */
typedef struct _Enesim_Point 	Enesim_Point; /**< */

/**
 *
 */
struct _Enesim_Point
{
	float x;
	float y;
};

/**
 *
 */
struct _Enesim_Path
{
	Edata_Array	*a;
	Enesim_Point 	*points;
	char 		*cmds;
	Enesim_Point 	*point_curr;
	char 		*cmd_curr;
	int 		num_vertices;

	void 		*data;
	void		(*alloc_cb)(void *data);
};

Enesim_Path * 	enesim_path_new(void *data, int vertices_ref);
void * 		enesim_path_delete(Enesim_Path *p);
void 		enesim_path_vertex_add(Enesim_Path *p, float x, float y, char cmd);

#endif
