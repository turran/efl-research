#include "openvg.h"

VG_API_CALL VGPath vgCreatePath(VGint pathFormat,
                                VGPathDatatype datatype,
                                VGfloat scale, VGfloat bias,
                                VGint segmentCapacityHint,
                                VGint coordCapacityHint,
                                VGbitfield capabilities)
{

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

