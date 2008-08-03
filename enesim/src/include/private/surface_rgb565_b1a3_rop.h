#ifndef SURFACE_RGB565_B1A3_ROP_H_
#define SURFACE_RGB565_B1A3_ROP_H_

static inline unsigned char rgb565_b1a3_alpha_get(uint16_t plane0, uint8_t plane1)
{
	return (plane1 & 0x3) << 5;
}

static inline unsigned char rgb565_b1a3_red_get(uint16_t plane0, uint8_t plane1)
{

}

static inline unsigned char rgb565_b1a3_green_get(uint16_t plane0, uint8_t plane1)
{

}

static inline unsigned char rgb565_b1a3_blue_get(uint16_t plane0, uint8_t plane1)
{

}

static inline void rgb565_b1a3_from_components(uint16_t *plane0, uint8_t *plane1, unsigned char a, unsigned char r,
		unsigned char g, unsigned char b)
{

}

static inline void rgb565_b1a3_to_components(uint16_t plane0, uint8_t plane1, unsigned char *a, unsigned char *r,
		unsigned char *g, unsigned char *b)
{

}

static inline void rgb565_b1a3_to_argb(unsigned int *argb, uint16_t plane0, 
uint8_t plane1, int pixel_plane1)
{	
	uint8_t alpha = rgb565_b1a3_alpha_get(plane0, plane1);
	
	rgb565_to_argb(argb, plane0, alpha);
}
static inline void rgb565_b1a3_from_argb(unsigned int argb, uint16_t *plane0, 
uint8_t *plane1)
{
	uint8_t alpha = argb8888_alpha_get(argb) >> 5;
	
	rgb565_from_argb(argb, plane0, alpha);
	*plane1 = alpha;
}

static inline void rgb565_b1a3_blend(uint16_t *dplane0, uint8_t *dplane1, int dpixel_plane1, uint16_t splane0, uint8_t splane1)
{
	uint8_t alpha;
	uint8_t dalpha;
	
	if (dpixel_plane1)
		dalpha = (*dplane1 >> 4) & 0x3;
	else
		dalpha = *dplane1 & 0x3;
	
	alpha = 32 - splane1;
	rgb565_blend(dplane0, splane0, alpha);
	/* TODO handle the pixel_plane var */
	*dplane1 = splane1 + ((alpha * dalpha) >> 3);
}

static inline void rgb565_b1a3_fill(uint16_t *dplane0, uint8_t *dplane1, int dpixel_plane1, uint16_t splane0, uint8_t splane1)
{
	rgb565_fill(dplane0, splane0);
	/* TODO handle the pixel_plane var */
	*dplane1 = splane1;
}

#endif /*SURFACE_RGB565_B1A3_ROP_H_*/
