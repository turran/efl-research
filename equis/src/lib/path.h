#ifndef _PATH_H
#define _PATH_H

typedef struct _Equis_Path 	Equis_Path; /**< */
typedef struct _Equis_Point 	Equis_Point; /**< */

/**
 *
 */
struct _Equis_Point
{
	float x;
	float y;
};

/**
 *
 */
struct _Equis_Path
{
	Equis_Point 	*points;
	char 		*cmds;
	Equis_Point 	*point_curr;
	char 		*cmd_curr;
	int 		num_allocated;
	int 		num_vertices;

	void 		*data;
	void		(*alloc_cb)(void *data);

};

Equis_Path * 	equis_path_new(void *data, int vertices_ref);
void * 		equis_path_delete(Equis_Path *p);
void 		equis_path_vertex_add(Equis_Path *p, float x, float y, char cmd);

#endif
