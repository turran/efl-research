#ifndef _X86_SSE_H
#define _X86_SSE_H

/* TODO
 * check this */
#include <xmmintrin.h>
#include "sse.h"

#define equis_t sse_t
#define equis_c_elements 4

#if 0

static inline void
sse_path_scale(sse_t *x, sse_t *y, sse_t *dx, sse_t *dy, int vertices, 
	float sx, float sy)
{
	int i;
	int num = (vertices / 4) + (vertices % 4 ? 1 : 0);
	sse_t sx4 = { 
		.s[0] = sx,
		.s[1] = sx,
		.s[2] = sx,
		.s[3] = sx
	};
	sse_t sy4 = { 
		.s[0] = sy,
		.s[1] = sy,
		.s[2] = sy,
		.s[3] = sy
	};
	
	for (i = 0; i < num; i++)
	{
		movaps_m2r(sx4, xmm1);
		mulps_m2r(*x, xmm1); 
		movaps_r2m(xmm1, *dx);

		dx++;
		x++;
	}
	for (i = 0; i < num; i++)
	{
		
		movaps_m2r(sy4, xmm1);
		mulps_m2r(*y, xmm1); 
		movaps_r2m(xmm1, *dy);
		
		dy++;
		y++;
	}
}

static inline void
sse_path_shear(sse_t *ct, sse_t *cnt, sse_t *dct, sse_t *dcnt, int vertices, 
	float k)
{
	int i;
	int num = (vertices / 4) + (vertices % 4 ? 1 : 0);
	sse_t *tmp_1, *tmp_2;

	sse_t k4 = { 
		.s[0] = k,
		.s[1] = k,
		.s[2] = k,
		.s[3] = k
	};
	
	tmp_1 = cnt;
	tmp_2 = dct;
	movaps_m2r(k4, xmm1);
	for (i = 0; i < num; i++)
	{
		movaps_m2r(*tmp_1, xmm2);
		mulps_r2r(xmm1, xmm2); 
		movaps_r2m(xmm2, *tmp_2);

		tmp_1++;
		tmp_2++;
	}
	for (i = 0; i < num; i++)
	{
		movaps_m2r(*dct, xmm1);
		addps_m2r(*ct, xmm1); 
		movaps_r2m(xmm1, *dct);

		ct++;
		dct++;
	}
	for (i = 0; i < num; i++)
	{
		movaps_m2r(*cnt, xmm1);
		movaps_r2m(xmm1, *dcnt);

		cnt++;
		dcnt++;
	}
}

static inline void
sse_path_vertex_add(sse_t *x, sse_t *y, float vx, float vy, int coffset)
{
	x->s[coffset] = vx;
	y->s[coffset] = vy;
}

static inline void
sse_path_vertex_get(sse_t *x, sse_t *y, float *vx, float *vy, int coffset)
{
	//printf("%f %d\n", x->s[coffset], coffset);
	*vx = x->s[coffset];
	*vy = y->s[coffset];
}

static inline int
sse_path_alloc(sse_t **x, sse_t **y, int num)
{
	int real_num = (num / 4) + 1;
	int bytes = sizeof(equis_t) * real_num;

	posix_memalign((void **)x, 16, bytes);
	posix_memalign((void **)y, 16, bytes);

	return bytes;
}

#define cpu_path_scale sse_path_scale
#define cpu_path_shear sse_path_shear
#define cpu_path_alloc sse_path_alloc
#define cpu_path_vertex_add sse_path_vertex_add
#define cpu_path_vertex_get sse_path_vertex_get

#endif

typedef struct
{
	sse_t *coords;
} sse_data;

static inline void
sse_path_transform(sse_data *s, sse_data *d, int vertices, 
	float *tm)
{
	int num = (vertices / 2) + (vertices % 2 ? 1 : 0);
	int i;
	sse_t *ts , *td;
	
	sse_t s1 = { 
		.s[0] = tm[0],
		.s[1] = tm[3],
		.s[2] = tm[0],
		.s[3] = tm[3]
	};
	sse_t s2 = { 
		.s[0] = tm[1],
		.s[1] = tm[2],
		.s[2] = tm[1],
		.s[3] = tm[2]
	};
	
	ts = s->coords;
	td = d->coords;
	
	movaps_m2r(s1, xmm1);
	movaps_m2r(s2, xmm2);
	for (i = 0; i < num; i++)
	{
		movaps_m2r(*ts, xmm3);
		mulps_r2r(xmm1, xmm3);
		movaps_m2r(*ts, xmm4);
		shufps(xmm4, xmm4, 0xb1);
		mulps_r2r(xmm2, xmm4);
		addps_r2r(xmm4, xmm3);
		movaps_r2m(xmm3, *td);
		ts++;
		td++;
	}
}

static inline void
sse_path_vertex_add(sse_data *d, int vertex, float vx, float vy)
{
	sse_t *t;
	int offset = 2 * (vertex % 2);
	int i;

	t = d->coords + (vertex / 2);
	t->s[offset] = vx;
	t->s[offset + 1] = vy;

}

static inline void
sse_path_vertex_get(sse_data *d, int vertex, float *vx, float *vy)
{
	sse_t *t;
	int offset = 2 * (vertex % 2);

	t = d->coords + (vertex / 2);
	*vx = t->s[offset];
	*vy = t->s[offset + 1];
}

static inline void
sse_path_new(void **data, int num)
{
	sse_data *d;

	*data = calloc(1, sizeof(sse_data));
	if (num)
	{
		d = *data;
		posix_memalign((void **)&d->coords, 16, sizeof(float) * num * 2);
	}
}


static inline void
sse_path_bezier(sse_t *d)
{

}

#define cpu_path_scale sse_path_scale
#define cpu_path_new sse_path_new
#define cpu_path_vertex_add sse_path_vertex_add
#define cpu_path_vertex_get sse_path_vertex_get
#define cpu_path_transform sse_path_transform

#endif
