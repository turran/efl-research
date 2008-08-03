#ifndef SURFACE_RGB565_XA5_ROP_H_
#define SURFACE_RGB565_XA5_ROP_H_

static inline unsigned char rgb565_xa5_alpha_get(uint16_t plane0, uint8_t plane1)
{
	return plane1 << 3;
}

static inline unsigned char rgb565_xa5_red_get(uint16_t plane0, uint8_t plane1)
{

}

static inline unsigned char rgb565_xa5_green_get(uint16_t plane0, uint8_t plane1)
{

}

static inline unsigned char rgb565_xa5_blue_get(uint16_t plane0, uint8_t plane1)
{

}

static inline void rgb565_xa5_from_components(uint16_t *plane0, uint8_t *plane1, unsigned char a, unsigned char r,
		unsigned char g, unsigned char b)
{

}

static inline void rgb565_xa5_to_components(uint16_t plane0, uint8_t plane1, unsigned char *a, unsigned char *r,
		unsigned char *g, unsigned char *b)
{

}

static inline void rgb565_xa5_to_argb(unsigned int *argb, uint16_t plane0, 
uint8_t plane1)
{	
	uint8_t alpha = (plane1 & 0x1f) << 3;
	
	rgb565_to_argb(argb, plane0, alpha);
}
static inline void rgb565_xa5_from_argb(unsigned int argb, uint16_t *plane0, 
uint8_t *plane1)
{
	uint8_t alpha = argb8888_alpha_get(argb) >> 3;
	
	rgb565_from_argb(argb, plane0, alpha);
	*plane1 = alpha;
}

static inline void rgb565_xa5_blend(uint16_t *dplane0, uint8_t *dplane1, uint16_t splane0, uint8_t splane1)
{
	uint8_t alpha;
	alpha = 32 - splane1;
	
	rgb565_blend(dplane0, splane0, alpha);
	*dplane1 = splane1 + ((alpha * (*dplane1)) >> 5);
}

static inline void rgb565_xa5_fill(uint16_t *dplane0, uint8_t *dplane1, uint16_t splane0, uint8_t splane1)
{
	rgb565_fill(dplane0, splane0);
	*dplane1 = splane1;
}

#endif /*SURFACE_RGB565_XA5_ROP_H_*/
