#include "Equanime.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _remove(Equanime_Layer *l)
{
	
}

static int _probe(Equanime_Layer *l)
{
	
}

static Equanime_Layer_Description mp25xxf_rgb_description = 
{
	.cname = "MagicEyes MP25XXF",
	.name = "RGB",
	.flags = EQUANIME_LAYER_VISIBILITY | EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE | EQUANIME_LAYER_LEVEL,
	.fncs = {
		.probe = &_probe,
		.remove = &_remove,	
	},
};

static int module_init(void)
{
	/* register the new layer */
	equanime_layer_register(&mp25xxf_rgb_description);
}

static void module_exit(void)
{
	/* unregister the layer */
	equanime_layer_unregister(&mp25xxf_rgb_description);
}
