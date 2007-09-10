#ifndef _X86_SSE_H
#define _X86_SSE_H

#define equis_t sse_t

typedef union _sse_t {
	long double q;	/* Quad precision (128-bit) 	*/
	float 	s[4]; 	/* 4 Single-precision (32-bit) 	*/
} __attribute__ ((aligned (16))) sse_t;

static inline void
cpu_path_scale(sse_t *x, sse_t *y, sse_t *dx, sse_t *dy, float sx, float sy, 
	int vertices)
{
	int i;
	int num = vertices / 4;
	int r = vertices % 4;
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
	#if 0
	/* assembler template 
           : output operands                  
           : input operands                   
           : list of clobbered registers      
           );
	*/
	#endif
	/* packed */
	printf("%f\n", dx->s[0]);
	asm (
		"movaps %%xmm0, %%xmm0"
	/*
		"movaps	%%xmm0,%1	\n\t"
		"mulps 	%%xmm0,%2	\n\t"
		"movaps	%0,%%xmm0	\n\t"
		:"=r"(dx)
		:"r"(x), "r"(sx)*/
	);
	printf("%f\n", dx->s[0]);
#if 0
	for (i = 0; i < num; i++)
	{
		dx->s[0] = x->s[0] * sx;
		dx->s[1] = x->s[1] * sx;
		dx->s[2] = x->s[2] * sx;
		dx->s[3] = x->s[3] * sx;
		
		dy->s[0] = y->s[0] * sy;
		dy->s[1] = y->s[1] * sy;
		dy->s[2] = y->s[2] * sy;
		dy->s[3] = y->s[3] * sy;
	}
#endif
	/* scalar */
	for (i = 0; i < r; i++)
	{

	}
}

#endif
