#include "openvg.h"
#include "private.h"

typedef struct _Evg_Path
{
	VGint format;
	VGPathDatatype datatype;
	VGfloat scale;
	VGfloat bias;
	VGbitfield capabilities;
	VGbyte *segments;
	void *data;
	/* implementation */
	Edata_Array *coord_allocator;
	int coord_num;
	Edata_Array *segment_allocator;
	int segment_num;
} Evg_Path;

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define SEGMENT_COMMAND(s) ((s &  0x1E) >> 1)

static const VGint _command_coords[] = {
		0, /* VG_CLOSE_PATH */
		2, /* VG_MOVE_TO */
		2, /* VG_LINE_TO */
		1, /* VG_HLINE_TO */
		1, /* VG_VLINE_TO */ 
		4, /* VG_QUAD_TO */
		6, /* VG_CUBIC_TO */
		2, /* VG_SQUAD_TO */
		4, /* VG_SCUBIC_TO */
		5, /* VG_SCCWARC_TO */
		5, /* VG_SCWARC_TO */
		5, /* VG_LCCWARC_TO */
		5, /* VG_LCWARC_TO */ 
};

static VGint _segment_coords_count(const VGubyte *s, VGint num)
{	
	int coords = 0;
	
	while (num)
	{
		short int cmd = SEGMENT_COMMAND(*s);
		
		if ((cmd < 0) || (cmd > 12))
			return -1;
		coords += _command_coords[cmd]; 
		s++;
		num--;
	}
	return coords;
}

static void _segment_alloc(Evg_Path *p, int num)
{
	
}

static void _segment_free(Evg_Path *p)
{
	
}

static void _coord_alloc(Evg_Path *p, int num)
{
	
}

static void _coord_free(Evg_Path *p)
{
	
}
	
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGPath vgCreatePath(VGint pathFormat,
                                VGPathDatatype datatype,
                                VGfloat scale, VGfloat bias,
                                VGint segmentCapacityHint,
                                VGint coordCapacityHint,
                                VGbitfield capabilities)
{
	Evg_Path *p = NULL;
	
	/* checks */
	EVG_RET_ERROR_IF((pathFormat != VG_PATH_FORMAT_STANDARD),
			VG_UNSUPPORTED_PATH_FORMAT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF((scale == 0), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	EVG_RET_ERROR_IF(((datatype < VG_PATH_DATATYPE_S_8) || (datatype > VG_PATH_DATATYPE_F)),
			VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
	
	p = calloc(1, sizeof(Evg_Path));
	EVG_RET_ERROR_IF((p == NULL),
				VG_OUT_OF_MEMORY_ERROR, VG_INVALID_HANDLE);
	
	p->format = pathFormat;
	p->datatype = datatype;
	p->scale = scale;
	p->bias = bias;
	p->capabilities = capabilities & VG_PATH_CAPABILITY_ALL;
	
	/* implementation */
	/* create the vertex container */
	p->segment_allocator = edata_array_new(p, EDATA_ARRAY_ALLOC(_segment_alloc),
			EDATA_ARRAY_FREE(_segment_free), segmentCapacityHint);
	p->coord_allocator = edata_array_new(p, EDATA_ARRAY_ALLOC(_coord_alloc),
			EDATA_ARRAY_FREE(_coord_free), coordCapacityHint);
	/* notify the context of a new path */
	evgCreatePath((VGPath)p);
	
	return ((VGPath)p);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgClearPath(VGPath path, VGbitfield capabilities)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDestroyPath(VGPath path)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgRemovePathCapabilities(VGPath path,
                                          VGbitfield capabilities)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGbitfield vgGetPathCapabilities(VGPath path)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgAppendPath(VGPath dstPath, VGPath srcPath)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgAppendPathData(VGPath dstPath,
                                  VGint numSegments,
                                  const VGubyte * pathSegments,
                                  const void * pathData)
{
	Evg_Path *p = (Evg_Path *)dstPath;
	int count;
	
	/* checks */
	EVG_ERROR_IF(evgPathExists(dstPath), VG_BAD_HANDLE_ERROR);
	EVG_ERROR_IF(!(p->capabilities & VG_PATH_CAPABILITY_APPEND_TO), VG_PATH_CAPABILITY_ERROR); 
	EVG_ERROR_IF(((pathSegments == NULL || pathData == NULL)), VG_ILLEGAL_ARGUMENT_ERROR);
	EVG_ERROR_IF(numSegments <= 0, VG_ILLEGAL_ARGUMENT_ERROR);
	
	/* TODO pathData is not properly aligned */
	
	/* pathSegments contains an illegal command */
	/* number of vertices to add based of segment type */
	count = _segment_coords_count(pathSegments, numSegments);
	EVG_ERROR_IF(count < 0, VG_ILLEGAL_ARGUMENT_ERROR);

	/* resize segment types and data */
	/* TODO check that we actually alloc the space */
	edata_array_elements_add(p->segment_allocator, count);
	edata_array_elements_add(p->coord_allocator, count);
	
	/* copy data */
	memcpy(p->segments + p->segment_num, pathSegments, numSegments);
	//memcpy((VGubyte *)p->coords + (p->coord_num * DATA_SIZE), pathData, count * DATA_SIZE);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgModifyPathCoords(VGPath dstPath, VGint startIndex,
                                    VGint numSegments,
                                    const void * pathData)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgTransformPath(VGPath dstPath, VGPath srcPath)
{


}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGboolean vgInterpolatePath(VGPath dstPath,
                                        VGPath startPath,
                                        VGPath endPath,
                                        VGfloat amount)
{


}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGfloat vgPathLength(VGPath path,
                                 VGint startSegment, VGint numSegments)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgPointAlongPath(VGPath path,
                                  VGint startSegment, VGint numSegments,
                                  VGfloat distance,
                                  VGfloat * x, VGfloat * y,
                                  VGfloat * tangentX, VGfloat * tangentY)
{


}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgPathBounds(VGPath path,
                              VGfloat * minX, VGfloat * minY,
                              VGfloat * width, VGfloat * height)
{


}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgPathTransformedBounds(VGPath path,
                                         VGfloat * minX, VGfloat * minY,
                                         VGfloat * width, VGfloat * height)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgDrawPath(VGPath path, VGbitfield paintModes)
{


}

