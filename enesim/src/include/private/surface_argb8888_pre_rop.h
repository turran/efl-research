#ifndef SURFACE_ARGB8888_H_
#define SURFACE_ARGB8888_H_

/* TODO this function can be split on two */
static inline void argb8888_pre_blend(unsigned int *dplane0, unsigned char alpha, unsigned int *splane0)
{
	
}

static inline void argb8888_pre_fill(unsigned int *dplane0, unsigned int *splane0)
{
	*dplane0 = *splane0;
}

#endif /*SURFACE_ARGB8888_H_*/
