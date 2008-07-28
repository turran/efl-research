#ifndef SURFACE_ARGB8888_ROP_H_
#define SURFACE_ARGB8888_ROP_H_


static inline unsigned int mul_256(unsigned char a, unsigned int c)
{
	return  ( (((((c) >> 8) & 0x00ff00ff) * (a)) & 0xff00ff00) +
	(((((c) & 0x00ff00ff) * (a)) >> 8) & 0x00ff00ff) );
}
/* Functions needed by the other subsystems */
static inline unsigned char argb8888_alpha_get(unsigned int plane0)
{
	return (plane0 >> 24);
}

static inline unsigned char argb8888_red_get(unsigned int plane0)
{
	return ((plane0 >> 16) & 0xff);
}

static inline unsigned char argb8888_green_get(unsigned int plane0)
{
	return ((plane0 >> 8) & 0xff);
}

static inline unsigned char argb8888_blue_get(unsigned int plane0)
{
	return (plane0 & 0xff);
}

static inline void argb8888_from_components(unsigned int *plane0, unsigned char a, unsigned char r,
		unsigned char g, unsigned char b)
{
	*plane0 = (a << 24) | (r << 16) | (g << 8) | b;
}

static inline void argb8888_to_components(unsigned int plane0, unsigned char *a, unsigned char *r,
		unsigned char *g, unsigned char *b)
{
	if (a) *a = argb8888_alpha_get(plane0);
	if (r) *r = argb8888_red_get(plane0);
	if (g) *g = argb8888_green_get(plane0);
	if (b) *b = argb8888_blue_get(plane0);
}
/* TODO swap this to/from argb functions with argb8888_unpremul */
static inline void argb8888_to_argb(unsigned int *argb, unsigned int plane0)
{
	unsigned int a = argb8888_alpha_get(plane0);
	
	if ((a > 1) && (a < 255))
	{
		unsigned char r, g, b;
		
		r = argb8888_red_get(plane0);
		g = argb8888_green_get(plane0);
		b = argb8888_blue_get(plane0);
		
		argb8888_from_components(argb, a, (r * 255) / a,  (g * 255) / a, (b * 255) / a);
	}
	else
		*argb = plane0;
}
static inline void argb8888_from_argb(unsigned int argb, unsigned int *plane0)
{
	unsigned int a = argb8888_alpha_get(argb) + 1;
	
	if (a != 256)
	{
		*plane0 = (argb & 0xff000000) + (((((argb) >> 8) & 0xff) * a) & 0xff00) +
		(((((argb) & 0x00ff00ff) * a) >> 8) & 0x00ff00ff);
	}
	else
		*plane0 = argb;
}

static inline void argb8888_blend(unsigned int *dplane0, unsigned int splane0)
{
	unsigned char a;
	
	a = 256 - argb8888_alpha_get(splane0);
	*dplane0 = splane0 + mul_256(a, *dplane0);
}

static inline void argb8888_fill(unsigned int *dplane0, unsigned int splane0)
{
	*dplane0 = splane0;
}

#endif /*SURFACE_ARGB8888_H_*/
