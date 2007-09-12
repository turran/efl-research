#ifndef _X86_H
#define _X86_H

/* TODO rename this file */
#if 0
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

typedef struct
{
	float *coords;
} c_data;

static inline void
c_path_transform(c_data *s, c_data *d, int vertices, 
	float *tm)
{
	int i;
	float *sf = s->coords;
	float *df = d->coords;
	
	for (i = 0; i < vertices; i++)
	{
		*df = (*sf * tm[0]) + (*(sf + 1) * tm[1]); // x
		*(df + 1) = (*sf * tm[2]) + (*(sf + 1) * tm[3]); // y
		
		sf += 2;
		df += 2;
	}
}

static inline void
c_path_vertex_add(c_data *d, int vertex, float vx, float vy)
{
	float *c;

	c = d->coords + (vertex  * 2);
	*c = vx;
	*(c + 1) = vy;
}

static inline void
c_path_vertex_get(c_data *d, int vertex, float *vx, float *vy)
{
	float *c;

	c = d->coords + (vertex  * 2);
	*vx = *c;
	*vy = *(c + 1);
}

static inline void
c_path_new(void **data, int num)
{
	c_data *d;

	*data = calloc(1, sizeof(c_data));
	if (num)
	{
		d = *data;
		d->coords = malloc(sizeof(float) * num * 2);
	}
}

#define cpu_path_scale 		c_path_scale
#define cpu_path_new 		c_path_new
#define cpu_path_vertex_add 	c_path_vertex_add
#define cpu_path_vertex_get 	c_path_vertex_get
#define cpu_path_transform 	c_path_transform

#endif
