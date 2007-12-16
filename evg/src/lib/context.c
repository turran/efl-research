#include "openvg.h"
#include "private.h"

typedef struct _Evg_Context
{
	VGErrorCode err;
} Evg_Context;

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Evg_Context *_context;
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
void evgSetError(VGErrorCode err)
{
	_context->err = err;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void evgCreatePath(VGPath p)
{
	/* append the path to the list of paths */
}
/**
 * To be documented
 * FIXME: To be fixed
 */
/* if dstPath is not a valid path handle, or is not shared with the current context */
VGboolean evgPathExists(VGPath p)
{
	return VG_TRUE;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGErrorCode vgGetError(void)
{
	return _context->err;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void evgContextSet(EVGContext c)
{
	_context = (Evg_Context *)c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL EVGContext evgContextGet(void)
{
	return ((EVGContext)_context);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL EVGContext evgContextNew(void)
{
	Evg_Context *e;
	
	e = calloc(1, sizeof(Evg_Context));
	return ((EVGContext)e);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void evgContextFree(EVGContext c)
{
	Evg_Context *e = (Evg_Context *)c;
	
	free(e);
}
