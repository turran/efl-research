/*
 * eon_cache.c
 *
 *  Created on: 16-jul-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"

#ifdef BUILD_CACHE_ESHM
#include "Eshm.h"
#endif
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#ifdef BUILD_CACHE_ESHM
static Eshm_Segment *_images;
#endif

typedef struct _Eon_Cache_Image
{
	const char *file;
	Enesim_Format f;
};

static void _loader_callback(Enesim_Surface *s, void *data, int error)
{
	/* TODO call the real callback registered */
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_cache_init(void)
{
#ifdef BUILD_CACHE_ESHM
	Eshm_Segment *s;

	/* initialize eshm */
	eshm_init();
	s = eshm_segment_new("eon:images", 64 * 1024 * 1024);
	/* if the segment already exists request it */
	if (!s)
	{
		s = eshm_segment_get("eon:images");
		if (!s)
			printf("[Eon_Cache] Image segment can't be created or retrieved\n");
	}
	_images = s;
#endif
}

void eon_cache_shutdown(void)
{
#ifdef BUILD_CACHE_ESHM
	/* shutdown eshm */
	eshm_shutdown();
#endif
}

void eon_cache_image_load(Enesim_Surface **s, const char *file,
		Enesim_Format f, Emage_Load_Callback cb, void *data,
		const char *options)
{
	char *key;
	Eina_Mempool *mp;
	Eon_Cache_Image *cim;

	/* setup our own memory pool */
	/* try to find our image on the cache */
	ky = "something based on the file, format and options";
	/* load the image in case it isnt found */
	cim = malloc(sizeof(Eon_Cache_Image));
	cim->cb = cb;
	cim->data = data;

	emage_load_async(file, s, f, NULL, _loader_callback, i, options);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

