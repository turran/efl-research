#include "Enesim.h"
#include "enesim_private.h"
#include "surface.h"

static inline DATA32 mul_256(DATA32 c, DATA32 a)
{
	return  ( (((((c) >> 8) & 0x00ff00ff) * (a)) & 0xff00ff00) +
	(((((c) & 0x00ff00ff) * (a)) >> 8) & 0x00ff00ff) );
}

void argb8888_color(Enesim_Surface_Data *data, int off, DATA32 c, int len)
{
	DATA32 *d, *e, a;

	d = data->argb8888.data + off;
	e = d + len;
	a = 256 - (c >> 24);

	while (d < e)
	{
		*d = c + mul_256(*d, a);
		d++;
	}
}

void argb8888_color_gradient(Enesim_Surface_Data *data, int off, DATA32 c,
	DATA8 a0, DATA8 a1, int w)
{

}

void color_alpha(Enesim_Surface_Data *data, int off, DATA32 c, DATA8 alpha,
int w)
{

}

void color_mask(Enesim_Surface_Data *data, int off, Enesim_Surface_Data *sdata, int soff, DATA32 c, DATA8 *mask, int w)
{

}

