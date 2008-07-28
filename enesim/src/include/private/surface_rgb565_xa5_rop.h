#ifndef SURFACE_RGB565_ROP_H_
#define SURFACE_RGB565_ROP_H_

static inline void rgb565_to_argb(unsigned int *argb, unsigned short int plane0, 
unsigned char plane1)
{
	*argb = ((plane0 & 0x1f) << 3) | ((plane0 & 0x7e0) << 5) | ((plane0 & 0xf800) << 8) | ((plane1 & 0x1f) << 27);
}
static inline void rgb565_from_argb(unsigned int argb, unsigned short int *plane0, 
unsigned char *plane1)
{
}

#endif /*SURFACE_RGB565_H_*/
