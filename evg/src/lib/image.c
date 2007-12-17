#include "openvg.h"
#include "private.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGImage vgCreateImage(VGImageFormat format,
                                  VGint width, VGint height,
                                  VGbitfield allowedQuality)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDestroyImage(VGImage image)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgClearImage(VGImage image,
                              VGint x, VGint y, VGint width, VGint height)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgImageSubData(VGImage image,
                                const void * data, VGint dataStride,
                                VGImageFormat dataFormat,
                                VGint x, VGint y, VGint width, VGint height)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetImageSubData(VGImage image,
                                   void * data, VGint dataStride,
                                   VGImageFormat dataFormat,
                                   VGint x, VGint y,
                                   VGint width, VGint height)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGImage vgChildImage(VGImage parent,
                                 VGint x, VGint y, VGint width, VGint height)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGImage vgGetParent(VGImage image)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgCopyImage(VGImage dst, VGint dx, VGint dy,
                             VGImage src, VGint sx, VGint sy,
                             VGint width, VGint height,
                             VGboolean dither)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDrawImage(VGImage image)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetPixels(VGint dx, VGint dy,
                             VGImage src, VGint sx, VGint sy,
                             VGint width, VGint height)
{
	
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgWritePixels(const void * data, VGint dataStride,
                               VGImageFormat dataFormat,
                               VGint dx, VGint dy,
                               VGint width, VGint height)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetPixels(VGImage dst, VGint dx, VGint dy,
                             VGint sx, VGint sy,
                             VGint width, VGint height)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgReadPixels(void * data, VGint dataStride,
                              VGImageFormat dataFormat,
                              VGint sx, VGint sy,
                              VGint width, VGint height)
{
	
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgCopyPixels(VGint dx, VGint dy,
                              VGint sx, VGint sy,
                              VGint width, VGint height)
{
	
	
}
