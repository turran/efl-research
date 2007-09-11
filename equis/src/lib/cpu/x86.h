#ifndef _X86_H
#define _X86_H

/* TODO rename this file */

#define equis_t float
#define equis_c_elements 1 

static inline void
x86_path_scale(float *x, float *y, float *dx, float *dy, int vertices, 
	float sx, float sy)
{
	int i;

	for (i = 0; i < vertices; i++)
	{
		*dx = *x * sx;
		*dy = *y * sy;

		x++;
		y++;
		dx++;
		dy++;
	}
}

static inline void
x86_path_shear(float *ct, float *cnt, float *dct, float *dcnt, int vertices, 
	float k)
{
	int i;

	for (i = 0; i < vertices; i++)
	{
		*dct = *ct + (*cnt * k);
		*dcnt = *cnt;

		ct++;
		cnt++;
		dct++;
		dcnt++;
	}
}

static inline int
x86_path_alloc(float **x, float **y, int num)
{
	int bytes = sizeof(float) * num;
	*x = malloc(bytes);
	*y = malloc(bytes);
	
	return bytes; 
}

static inline void
x86_path_vertex_add(float *x, float *y, float vx, float vy, int coffset)
{
	*x = vx;
	*y = vy;
}

static inline void
x86_path_vertex_get(float *x, float *y, float *vx, float *vy, int coffset)
{
	*vx = *x;
	*vy = *y;
}

#define cpu_path_scale x86_path_scale
#define cpu_path_shear x86_path_shear
#define cpu_path_alloc x86_path_alloc
#define cpu_path_vertex_add x86_path_vertex_add
#define cpu_path_vertex_get x86_path_vertex_get

#endif
