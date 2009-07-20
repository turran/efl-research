#include "openvg.h"
#include "private.h"

typedef struct
{
	VGPaintType type;
	VGfloat color[4];
	VGColorRampSpreadMode ramp_spread_pad;
	VGfloat *ramp_stops;
	VGboolean ramp_multiplied;
	VGfloat linear_gradient[4];
	VGfloat radial_gradient[5];
	VGTilingMode tiling_mode;
} Evg_Paint;

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
VGint evgPaintGetParameteri(VGPaint p, VGint paramType)
{
	
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGPaint vgCreatePaint(void)
{
	Evg_Paint *p;
	
	p = malloc(sizeof(Evg_Paint));
	/* notify the context of a new paint */
	evgCreatePaint((VGPaint)p);
	return (VGPaint)p;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDestroyPaint(VGPaint paint)
{
	Evg_Paint *p;
	
	p = (Evg_Paint *)paint;
	free(p);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetColor(VGPaint paint, VGuint rgba)
{
	Evg_Paint *p;
			
	p = (Evg_Paint *)paint;	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGuint vgGetColor(VGPaint paint)
{
	Evg_Paint *p;
			
	p = (Evg_Paint *)paint;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgPaintPattern(VGPaint paint, VGImage pattern)
{
	Evg_Paint *p;
			
	p = (Evg_Paint *)paint;
}
