#ifndef _X86_SSE_H
#define _X86_SSE_H

#define equis_t sse_t

typedef union _sse_t {
	long double q;	/* Quad precision (128-bit) 	*/
	float 	s[4]; 	/* 4 Single-precision (32-bit) 	*/
} __attribute__ ((aligned (16))) sse_t;

#endif
