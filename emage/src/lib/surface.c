#include "Emage.h"
#include "emage_private.h"

/* Surface
 * ~~~~~~~
 * Alls this _set, get are needed or better export the whole struct through
 * the public header.
 *
 */

EAPI Emage_Surface *
emage_surface_new(void *data, Emage_Data_Format f, int w, int h)
{
	Emage_Surface *s;

	s = calloc(1, sizeof(Emage_Surface));
	s->w = w;
	s->h = h;
	s->data = data;
	s->format = f;
	s->flags = RGBA_SURFACE_HAS_ALPHA;

	return s;
}

EAPI void
emage_surface_size_get(Emage_Surface *s, int *w, int *h)
{
	assert(s);
	if (w) *w = s->w;
	if (h) *h = s->h;
}

EAPI void *
emage_surface_data_get(Emage_Surface *s)
{
	assert(s);
	return s->data;
}

EAPI void 
emage_surface_data_set(Emage_Surface *s, void *data)
{
	assert(s);
	s->data = data;
}
