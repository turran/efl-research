#ifndef SURFACE_ARGB8888_PRE_ROP_H_
#define SURFACE_ARGB8888_PRE_ROP_H_


static inline unsigned int mul_256(unsigned char a, unsigned int c)
{
	return  ( (((((c) >> 8) & 0x00ff00ff) * (a)) & 0xff00ff00) +
	(((((c) & 0x00ff00ff) * (a)) >> 8) & 0x00ff00ff) );
}
/* Functions needed by the other subsystems */
static inline void argb8888_pre_to_argb(unsigned int *argb, unsigned int plane0)
{
	*argb = ((plane0 & 0xff) << 0) | ((plane0 & 0xff00) << 0) | ((plane0 & 0xff0000) << 0) | ((plane0 & 0xff000000) << 0);
}
static inline void argb8888_pre_from_argb(unsigned int argb, unsigned int *plane0)
{
	unsigned char a = (argb >> 24) + 1;
	
	*plane0 = mul_256(a, argb);
}
/* TODO this function can be split on two */
static inline void argb8888_pre_blend(unsigned int *dplane0, unsigned int splane0)
{
	unsigned char a;
	
	a = 256 - (splane0 >> 24);
	*dplane0 = splane0 + mul_256(a, *dplane0);
}

static inline void argb8888_pre_fill(unsigned int *dplane0, unsigned int splane0)
{
	*dplane0 = splane0;
}

#endif /*SURFACE_ARGB8888_H_*/
