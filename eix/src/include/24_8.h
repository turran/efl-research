#ifndef _EIX_24_8_H
#define _EIX_24_8_H

/**
 * 24.8
 * 16 777 216.256
 */
typedef int eix;

static inline eix eix_from_int(int v)
{
	eix r = v << 8;
	return r;
}

static inline eix eix_from_float(float v)
{
	eix r;
	
	r = (eix)(v * 256.0 + (v < 0 ? -0.5 : 0.5));
	return r;
}

static inline float eix_to_float(eix v)
{
	float r;
	
	r = v/256.0;
	return r;
}

static inline eix eix_add(eix a, eix b)
{
	eix r = a + b;
#ifdef _EIX_OVERFLOW
#else
	return r;
#endif
}

static inline eix eix_sub(eix a, eix b)
{
	eix r = a - b;
#ifdef _EIX_OVERFLOW
#else
	return r;
#endif
}

static inline eix eix_mul(eix a, eix b)
{
	long int r = a * b;
#ifdef _EIX_OVERFLOW
#else
	return r >> 8;
#endif
}

#endif
