#include "Emage.h"
#include "emage_private.h"

EAPI RGBA_Surface *
emage_surface_new(void *data, int w, int h)
{
	RGBA_Surface *s;

	s = malloc(sizeof(RGBA_Surface));
	s->w = w;
	s->h = h;
	s->data = data;
	s->flags = RGBA_SURFACE_HAS_ALPHA;

	return s;
}

EAPI emage_surface_size_get(RGBA_Surface *s, int *w, int *h)
{
	if (!s) return;
	if (w) *w = s->w;
	if (h) *h = s->h;
}
