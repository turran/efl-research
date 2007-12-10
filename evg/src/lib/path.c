#include <stdio.h>
#include "openvg.h"
#include "Enesim.h"

typedef struct _Evg_Path
{
	VGint format;
	VGPathDatatype datatype;
	VGfloat scale;
	VGfloat bias;
	VGbitfield capabilities;
	void *client_data;
	void *impl_data;
} Evg_Path;

/**
 * @errors
 * VG_UNSUPPORTED_PATH_FORMAT_ERROR if pathFormat is not a supported format
 * VG_ILLEGAL_ARGUMENT_ERROR if datatype is not a valid value from the 
 * VGPathDatatype enumeration or if scale is equal to 0
 */
VG_API_CALL VGPath vgCreatePath(VGint pathFormat,
                                VGPathDatatype datatype,
                                VGfloat scale, VGfloat bias,
                                VGint segmentCapacityHint,
                                VGint coordCapacityHint,
                                VGbitfield capabilities)
{
	Evg_Path *p = NULL;
	
	if (pathFormat != VG_PATH_FORMAT_STANDARD)
		
	p = calloc(1, sizeof(Evg_Path));
	
	p->format = pathFormat;
	p->datatype = datatype;
	p->scale = scale;
	p->bias = bias;
	p->capabilities = capabilities & VG_PATH_CAPABILITY_ALL;
	
	return ((VGPath)p);
}

VG_API_CALL void vgClearPath(VGPath path, VGbitfield capabilities)
{

}

VG_API_CALL void vgDestroyPath(VGPath path)
{

}

VG_API_CALL void vgRemovePathCapabilities(VGPath path,
                                          VGbitfield capabilities)
{

}

VG_API_CALL VGbitfield vgGetPathCapabilities(VGPath path)
{

}

VG_API_CALL void vgAppendPath(VGPath dstPath, VGPath srcPath)
{

}

VG_API_CALL void vgAppendPathData(VGPath dstPath,
                                  VGint numSegments,
                                  const VGubyte * pathSegments,
                                  const void * pathData)
{

}

VG_API_CALL void vgModifyPathCoords(VGPath dstPath, VGint startIndex,
                                    VGint numSegments,
                                    const void * pathData)
{

}

VG_API_CALL void vgTransformPath(VGPath dstPath, VGPath srcPath)
{


}

VG_API_CALL VGboolean vgInterpolatePath(VGPath dstPath,
                                        VGPath startPath,
                                        VGPath endPath,
                                        VGfloat amount)
{


}

VG_API_CALL VGfloat vgPathLength(VGPath path,
                                 VGint startSegment, VGint numSegments)
{

}

VG_API_CALL void vgPointAlongPath(VGPath path,
                                  VGint startSegment, VGint numSegments,
                                  VGfloat distance,
                                  VGfloat * x, VGfloat * y,
                                  VGfloat * tangentX, VGfloat * tangentY)
{


}

VG_API_CALL void vgPathBounds(VGPath path,
                              VGfloat * minX, VGfloat * minY,
                              VGfloat * width, VGfloat * height)
{


}
VG_API_CALL void vgPathTransformedBounds(VGPath path,
                                         VGfloat * minX, VGfloat * minY,
                                         VGfloat * width, VGfloat * height)
{

}

VG_API_CALL void vgDrawPath(VGPath path, VGbitfield paintModes)
{


}

