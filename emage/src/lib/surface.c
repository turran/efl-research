#include "Emage.h"
#include "emage_private.h"

/* Surface
 * ~~~~~~~
 * Alls this _set, get are needed or better export the whole struct through
 * the public header.
 *
 */

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

EAPI inline void
emage_surface_size_get(RGBA_Surface *s, int *w, int *h)
{
	assert(s);
	if (w) *w = s->w;
	if (h) *h = s->h;
}

EAPI inline void *
emage_surface_data_get(RGBA_Surface *s)
{
	assert(s);
	return s->data;
}

EAPI inline void 
emage_surface_data_set(RGBA_Surface *s, void *data)
{
	assert(s);
	s->data = data;
}
