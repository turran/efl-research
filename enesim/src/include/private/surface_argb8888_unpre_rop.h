#ifndef SURFACE_ARGB8888_UNPRE_ROP_H_
#define SURFACE_ARGB8888_UNPRE_ROP_H_

#define BLEND_ARGB_256(a, aa, c0, c1) \
 ( ((((0xff0000 - (((c1) >> 8) & 0xff0000)) * (a)) \
   + ((c1) & 0xff000000)) & 0xff000000) + \
   (((((((c0) >> 8) & 0xff) - (((c1) >> 8) & 0xff)) * (aa)) \
   + ((c1) & 0xff00)) & 0xff00) + \
   (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (aa)) >> 8) \
   + ((c1) & 0xff00ff)) & 0xff00ff) )

static inline unsigned char argb8888_unpre_alpha_get(unsigned int plane0)
{
	return (plane0 >> 24);
}

static inline unsigned char argb8888_unpre_red_get(unsigned int plane0)
{
	return ((plane0 >> 16) & 0xff);
}

static inline unsigned char argb8888_unpre_green_get(unsigned int plane0)
{
	return ((plane0 >> 8) & 0xff);
}

static inline unsigned char argb8888_unpre_blue_get(unsigned int plane0)
{
	return (plane0 & 0xff);
}

static inline void argb8888_unpre_from_components(unsigned int *plane0, unsigned char a, unsigned char r,
		unsigned char g, unsigned char b)
{
	*plane0 = (a << 24) | (r << 16) | (g << 8) | b;
}

static inline void argb8888_unpre_to_components(unsigned int plane0, unsigned char *a, unsigned char *r,
		unsigned char *g, unsigned char *b)
{
	if (a) *a = argb8888_unpre_alpha_get(plane0);
	if (r) *r = argb8888_unpre_red_get(plane0);
	if (g) *g = argb8888_unpre_green_get(plane0);
	if (b) *b = argb8888_unpre_blue_get(plane0);
}

static inline void argb8888_unpre_to_argb(unsigned int *argb, unsigned int plane0)
{
	unsigned int a = argb8888_unpre_alpha_get(plane0) + 1;
		
	if (a != 256)
	{
		*argb = (plane0 & 0xff000000) + (((((plane0) >> 8) & 0xff) * a) & 0xff00) +
		(((((plane0) & 0x00ff00ff) * a) >> 8) & 0x00ff00ff);
	}
	else
		*argb = plane0;
}
static inline void argb8888_unpre_from_argb(unsigned int argb, unsigned int *plane0)
{
	unsigned int a = argb8888_alpha_get(argb);
	if ((a > 1) && (a < 255))
	{
		unsigned char r, g, b;
			
		r = argb8888_red_get(argb);
		g = argb8888_green_get(argb);
		b = argb8888_blue_get(argb);
			
		argb8888_from_components(plane0, a, (r * 255) / a,  (g * 255) / a, (b * 255) / a);
	}
	else
		*plane0 = argb;
}

static inline void argb8888_unpre_blend(unsigned int *dplane0, unsigned int splane0)
{
	/* TODO this is really wrong! */
	*dplane0 = BLEND_ARGB_256(1 + (splane0 >> 24), 1 + (*dplane0 >> 24), splane0, *dplane0);
}

static inline void argb8888_unpre_fill(unsigned int *dplane0, unsigned int splane0)
{
	*dplane0 = splane0;
}

#endif /*SURFACE_ARGB8888_H_*/
