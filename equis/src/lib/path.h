#ifndef _PATH_H
#define _PATH_H

struct _Equis_Path
{
	void *cpu_data;
	//equis_t *x;
	//equis_t *y;
	char *cmd;

	int num_allocated;
	int num_vertices;

	int next_vertex;
};

#define MATRIX_IS_SCALE(m) 	((!tm[1]) && (!tm[2]))
#define MATRIX_IS_SHEAR_X(m) 	((tm[0] == 1) && (!tm[2]) && (tm[3] == 1))
#define MATRIX_IS_SHEAR_Y(m) 	((tm[0] == 1) && (!tm[1]) && (tm[3] == 1))
#define MATRIX_IS_IDENTITY(m) 	((tm[0] == 1) && (!tm[1]) &&		\
				(!tm[2]) && (tm[3] == 1))


#endif
