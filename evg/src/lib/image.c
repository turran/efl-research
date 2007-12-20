#include "openvg.h"
#include "private.h"

struct _Evg_Image
{
	/* common */
	VGint width;
	VGint height;
	VGImageFormat format;
	VGbitfield allowed_quality;
	
	VGint refcount;
	VGbyte delete_me;
	/* implementation */
	Enesim_Surface *s;
	void *data;
};

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define EVG_IMAGE_FORMAT(c) (c & 0xff)

static const VGint _format_bytes[] = {
	4, /* sRGBX_8888 */
	4, /* sRGBA_8888 */
	4, /* sRGBA_8888_PRE */
	2, /* sRGB_565 */
	2, /* sRGBA_5551 */
	2, /* sRGBA_4444 */
	1, /* sL_8 */
	4, /* lRGBX_8888 */
	4, /* lRGBA_8888 */
	4, /* lRGBA_8888_PRE */
	1, /* lL_8 */
	1, /* A_8 */
	0 /* BW_1 */
};

void _delete(Evg_Image *im)
{
	enesim_surface_delete(im->s);
	free(im->data);
	free(im);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * If the image is currently in use as a rendering target, is the ancestor
 * of another image (see vgChildImage), or is set as a paint pattern image
 * on a VGPaint object, its definition remains available to those consumers
 * as long as they remain valid, but the handle may no longer be used.
 * When those uses cease, the image’s resources will automatically be deallocated.
 * FIXME: To be fixed
 */
void evgImageUnref(VGImage image)
{
	Evg_Image *im = (Evg_Image *)image;
	im->refcount--;
	if ((!im->refcount) && (im->delete_me))
	{
		_delete(im);
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */ 
void evgImageRef(VGImage image)
{
	Evg_Image *im = (Evg_Image *)image;
	im->refcount++;
}
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
	Evg_Image *im;
	 
	EVG_RET_ERROR_IF((width <= 0), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF((width > VG_MAX_IMAGE_WIDTH), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF((height <= 0), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF((height > VG_MAX_IMAGE_HEIGHT), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF(((height * width) > VG_MAX_IMAGE_PIXELS), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF(((height * width * _format_bytes[EVG_IMAGE_FORMAT(format)]) > VG_MAX_IMAGE_BYTES), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	
	/* TODO
	 * if allowedQuality is not a bitwise OR of values from the VGImageQuality enumeration */ 
	
	switch (format)
	{
	case  VG_lARGB_8888_PRE:
	{
		Enesim_Surface *s;
		DATA32 *d;
		
		d = calloc(1, sizeof(DATA32) * width * height);
		s = enesim_surface_new(ENESIM_SURFACE_ARGB8888, width, height, ENESIM_SURFACE_ALPHA, d);
		im->s = s;
		break;
	}	
	default:
		/* return format unavailable */
		EVG_RET_ERROR(VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_INVALID_HANDLE);
	}
	im->width = width;
	im->height = height;
	im->format = format;
	im->allowed_quality = allowedQuality;
	
	/* notify the context of a new iamge */
	evgCreateImage((VGImage)im);
	evgImageRef((VGImage)im);
	return ((VGImage)im);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDestroyImage(VGImage image)
{
	Evg_Image *im = (Evg_Image *)image;
	im->delete_me = 1;
	evgImageUnref(image);
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
