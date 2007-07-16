#include "Emage.h"

static int init = 0;

EAPI int emage_init(void)
{
	if (++init != 1) return init;
	evas_common_cpu_init();

	evas_common_blend_init();
	evas_common_image_init();
	evas_common_convert_init();
	evas_common_scale_init();
	evas_common_gradient_init();
	evas_common_polygon_init();
	evas_common_font_init();
	evas_common_draw_init();
	evas_common_tilebuf_init();

	return init;
}

EAPI void emage_shutdown(void)
{
	if (--init) return;
}


EAPI RGBA_Surface *
emage_surface_new(void *data, int w, int h)
{
	RGBA_Surface *s;

	s = malloc(sizeof(RGBA_Surface));
	s->w = w;
	s->h = h;
	s->data = data;

	return s;
}

