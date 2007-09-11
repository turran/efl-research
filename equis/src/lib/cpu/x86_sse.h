#ifndef _X86_SSE_H
#define _X86_SSE_H

#define equis_t sse_t
#define equis_c_elements 4

typedef union _sse_t {
	long double 		qf;	/* Quad precision (128-bit) 	*/
	long long 		q[2];	/* 2 Quadword (64-bit) */
	unsigned long long 	uq[2];	/* 2 Unsigned Quadword */
	int 			d[4];	/* 4 Doubleword (32-bit) */
	unsigned int 		ud[4];	/* 4 Unsigned Doubleword */
	short 			w[8];	/* 8 Word (16-bit) */
	unsigned short 		uw[8];	/* 8 Unsigned Word */
	char 			b[16];	/* 16 Byte (8-bit) */
	unsigned char 		ub[16];	/* 16 Unsigned Byte */
	float 			s[4]; 	/* 4 Single-precision (32-bit) 	*/
} __attribute__ ((aligned (16))) sse_t;

#define	sse_i2r(op, imm, reg) 						\
	__asm__ __volatile__ (#op " $" #imm ", %%" #reg 		\
			      : /* nothing */ 				\
			      : /* nothing */);

#define	sse_m2r(op, mem, reg) 						\
	__asm__ __volatile__ (#op " %0, %%" #reg 			\
			      : /* nothing */ 				\
			      : "m" (mem))

#define	sse_r2m(op, reg, mem) 						\
	__asm__ __volatile__ (#op " %%" #reg ", %0" 			\
			      : "=m" (mem) 				\
			      : /* nothing */ )

#define	sse_a2r(op, mem, reg) 						\
	__asm__ __volatile__ (#op " %0, %%" #reg 			\
			      : /* nothing */ 				\
			      : "m" (mem))

#define	sse_r2a(op, reg, mem) 						\
	__asm__ __volatile__ (#op " %%" #reg ", %0" 			\
			      : "=m" (mem) \
			      : /* nothing */ )

#define	sse_r2r(op, regs, regd) \
	__asm__ __volatile__ (#op " %" #regs ", %" #regd)

/*
 * Scalar - MOVSS
 * Packed - MOVAPS, MOVUPS, MOVLPS, MOVHPS, MOVLHPS, MOVHLPS
 */

#define	movaps_m2r(var, reg)	sse_a2r(movaps, var, reg)
#define	movaps_r2m(reg, var)	sse_r2a(movaps, reg, var)
#define	movaps_r2r(regs, regd)	sse_r2r(movaps, regs, regd)

#define	movups_m2r(var, reg)	sse_a2r(movups, var, reg)
#define	movups_r2m(reg, var)	sse_r2a(movups, reg, var)
#define	movups_r2r(regs, regd)	sse_r2r(movups, regs, regd)

/*
 * Scalar - ADDSS, SUBSS, MULSS, DIVSS, RCPSS, SQRTSS, MAXSS, MINSS, RSQRTSS
 * Packed - ADDPS, SUBPS, MULPS, DIVPS, RCPPS, SQRTPS, MAXPS, MINPS, RSQRTPS
 */

#define	mulps_m2r(var, reg)	sse_m2r(mulps, var, reg)
#define	mulps_r2r(regs, regd)	sse_r2r(mulps, regs, regd)

#define	addps_m2r(var, reg)	sse_m2r(addps, var, reg)
#define	addps_r2r(regs, regd)	sse_r2r(addps, regs, regd)

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
