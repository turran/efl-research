#include "Emage.h"
#include "emage_private.h"

#include <stdlib.h>

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _sampled_func(Emage_Surface *src, Emage_Rectangle *sr, 
	Emage_Surface *dst, Emage_Rectangle *dr, Emage_Draw_Context *dc,
	Emage_Rectangle *cr)
{
	int y, x, doffset, *offset_x, *offset_y;

	/* pre-calculated scale tables */
	offset_x = alloca(cr->w * sizeof(*offset_x));
	for (x = 0; x < cr->w; x++)
		offset_x[x] = (((x + cr->x - dr->x) * sr->w) / dr->w) + sr->x;

	offset_y = alloca(cr->h * sizeof(*offset_y));
	for (y = 0; y < cr->h; y++)
		offset_y[y] = (((((y + cr->y - dr->y) * sr->h) / dr->h) + sr->y)
				* src->w);
	
	doffset = (cr->y * dst->w) + cr->x;
	for (y = 0; y < cr->h; y++)
	{
		printf("drawing in %d %d\n", dr->x, y);
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void emage_scaler_sampled_init(void)
{
#ifdef BUILD_SCALE_SAMPLE
	Emage_Scalers[EMAGE_SCALER_SAMPLED].func = _sampled_func;
#endif
}
