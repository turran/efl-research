#ifndef SURFACE_RGB565_ROP_H_
#define SURFACE_RGB565_ROP_H_

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
unsigned char plane1)
{
	*argb = ((plane0 & 0x1f) << 3) | ((plane0 & 0x7e0) << 5) | ((plane0 & 0xf800) << 8) | ((plane1 & 0x1f) << 27);
}
static inline void rgb565_xa5_from_argb(unsigned int argb, uint16_t *plane0, 
uint8_t *plane1)
{
	//*plane0 = 
	*plane1 = argb8888_alpha_get(argb) >> 3;
}

static inline void rgb565_xa5_blend(uint16_t *dplane0, uint8_t *dplane1, uint16_t splane0, uint16_t splane1)
{

}

static inline void rgb565_xa5_fill(uint16_t *dplane0, uint8_t *dplane1, uint16_t splane0, uint8_t plane1)
{

}

#endif /*SURFACE_RGB565_H_*/
