#ifndef EIX_24P8_H
#define EIX_24P8_H

/**
 * @file 
 * @brief 
 * 24.8 precision = +- 8388607.255
 *
 */
#define eix24p8_maxi 	0x7fffff
#define eix24p8_mini 	-0x7fffff

#define eix24p8_maxf 	8388607.255
#define eix24p8_minf 	-8388607.255

#define eix24p8_max 	0x7fffffff
#define eix24p8_min 	-0x7fffffff

typedef int eix24p8;

static inline eix24p8 eix_24p8_from_int(int v)
{
#ifdef EIX_OVERFLOW
	if (v > eix24p8_maxi)
		return eix24p8_max;
	if (v < eix24p8_mini)
		return eix24p8_min;
#endif
	return v << 8;
}

static inline eix24p8 eix_24p8_from_float(float v)
{
	eix24p8 r;
	
#ifdef EIX_OVERFLOW
	if (v > eix24p8_maxf)
		return eix24p8_max;
	if (v < eix24p8_minf)
		return eix24p8_min;
#endif
	r = (eix24p8)(v * 256.0 + (v < 0 ? -0.5 : 0.5));
	return r;
}

static inline float eix_24p8_to_float(eix24p8 v)
{
	float r;
	
	r = v / 256.0;
	return r;
}

static inline eix24p8 eix_24p8_add(eix24p8 a, eix24p8 b)
{
	eix24p8 r = a + b;
#ifdef EIX_OVERFLOW
#else
#endif
	return r;
}

static inline eix24p8 eix_24p8_sub(eix24p8 a, eix24p8 b)
{
	eix24p8 r = a - b;
#ifdef EIX_OVERFLOW
#else
#endif
	return r;
}

static inline eix24p8 eix_24p8_mul(eix24p8 a, eix24p8 b)
{
	long int r = a * b;
#ifdef EIX_OVERFLOW
#else
#endif
	return r >> 8;
}

static inline eix24p8 eix_24p8_div(eix24p8 a, eix24p8 b)
{
#ifdef EIX_OVERFLOW
#else
#endif
	return a;
}

#ifdef EIX_24P8
#define eix 		eix24p8
#define eix_from_int 	eix_24p8_from_int
#define eix_from_float 	eix_24p8_from_float
#define eix_to_float 	eix_24p8_to_float
#define eix_add 	eix_24p8_add
#define eix_sub 	eix_24p8_sub
#define eix_mul 	eix_24p8_mul
#define eix_div 	eix_24p8_div
#endif

#endif
