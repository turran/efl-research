#ifndef _PATH_H
#define _PATH_H

struct _Equis_Path
{
	equis_t *x;
	equis_t *y;
	char *cmd;

	int num_allocated;
	int num_vertices;
};

#endif
