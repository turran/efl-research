#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Emage_Scaler scaler_default = {
	.func 	= dummy_scale
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Emage_Scaler Emage_Scalers[EMAGE_SCALER_TYPES];

void emage_scaler_init(void)
{
	int i;

	for (i = 0; i < EMAGE_SCALER_TYPES; i++)
	{
		Emage_Scalers[i] = scaler_default;
	}
	emage_scaler_sampled_init();
	emage_scaler_smooth_init();
}

void dummy_scale(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc)
{
	PRINTF("not implemented\n");
}
