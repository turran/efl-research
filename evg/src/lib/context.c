#include "openvg.h"
#include "private.h"

#define EVG_MATRIX_MODES 4
#define EVG_MATRIX_VALUES 9 /* 3x3 matrices */

typedef struct _Evg_Context
{
	struct
	{
		VGboolean format_linear;
		VGboolean format_premultiplied;
		VGbitfield channel_mask;	
	} filter;
	struct
	{
		VGfloat tile_fill_color[4];
		VGfloat clear_color[4];
		struct
		{
			VGfloat line_width;
			VGCapStyle cap_style;
			VGJoinStyle join_style;
			VGfloat mitter_limit;
			VGfloat *dash_pattern;
			VGfloat dash_phase;
			VGboolean dash_phase_reset;
		} stroke;
		struct
		{
			VGFillRule rule;
		} fill;
	} paint;
	struct
	{
		VGBlendMode mode;	
	} blend;
	struct
	{
		VGImageMode mode;
		VGImageQuality quality;
	} image;
	struct
	{
		VGRenderingQuality quality;
		VGPixelLayout pixel_layout;
		struct
		{
			VGMatrixMode mode;
			VGfloat matrices[EVG_MATRIX_MODES][EVG_MATRIX_VALUES];
		} matrix;
	} rendering;
	struct
	{
		Evg_Image *surface;
		VGPixelLayout screen_layout;
	} target;
	VGboolean scissoring;
	VGint *scissor_rects;
	VGboolean masking;
	/* Error */
	VGErrorCode err;
	/* implementation */
	Edata_Hash *path_hash;
	Edata_Hash *image_hash;
	Edata_Hash *paint_hash;
	
} Evg_Context;

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Evg_Context *_context;

static void _get_handle_type(VGHandle h)
{
	
}

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
	edata_hash_set(_context->path_hash, (void *)p, (void *)p);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void evgCreateImage(VGImage i)
{
	edata_hash_set(_context->image_hash, (void *)i, (void *)i);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void evgCreatePaint(VGPaint p)
{
	edata_hash_set(_context->paint_hash, (void *)p, (void *)p);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
/* if dstPath is not a valid path handle, or is not shared with the current context */
VGboolean evgPathExists(VGPath p)
{
	if (edata_hash_get(_context->path_hash, (void *)p))
		return VG_TRUE;
	else
		return VG_FALSE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VGboolean evgImageExists(VGImage i)
{
	if (edata_hash_get(_context->image_hash, (void *)i))
		return VG_TRUE;
	else
		return VG_FALSE;	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VGboolean evgPaintExists(VGPaint p)
{
	if (edata_hash_get(_context->paint_hash, (void *)p))
		return VG_TRUE;
	else
		return VG_FALSE;
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
VG_API_CALL void vgSetf (VGParamType type, VGfloat value)
{
	switch (type)
	{
		case VG_STROKE_LINE_WIDTH:
		break;
		case VG_STROKE_MITER_LIMIT:
		break;
		case VG_STROKE_DASH_PHASE:
		break;
		default:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSeti (VGParamType type, VGint value)
{
	switch (type)
	{
		case VG_BLEND_MODE:
		break;
		case VG_IMAGE_MODE:
		break;
		case VG_IMAGE_QUALITY:
		break;
		case VG_STROKE_DASH_PHASE_RESET:
		break;
		case VG_STROKE_JOIN_STYLE:
		break;
		case VG_STROKE_CAP_STYLE:
		break;
		case VG_FILL_RULE:
		break;
		case VG_SCISSORING:
		break;
		case VG_MATRIX_MODE:
		break;
		case VG_RENDERING_QUALITY:
		break;
		
		default:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetfv(VGParamType type, VGint count,
                         const VGfloat * values)
{
	switch (type)
	{
		case VG_CLEAR_COLOR:
		break;

		case VG_STROKE_DASH_PATTERN:
		break;

		default:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetiv(VGParamType type, VGint count,
                         const VGint * values)
{
	switch(type)
	{
		case VG_SCISSOR_RECTS:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGfloat vgGetf(VGParamType type)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGint vgGeti(VGParamType type)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGint vgGetVectorSize(VGParamType type)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetfv(VGParamType type, VGint count, VGfloat * values)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetiv(VGParamType type, VGint count, VGint * values)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetParameterf(VGHandle object,
                                 VGint paramType,
                                 VGfloat value)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetParameteri(VGHandle object,
                                 VGint paramType,
                                 VGint value)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetParameterfv(VGHandle object,
                                  VGint paramType,
                                  VGint count, const VGfloat * values)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetParameteriv(VGHandle object,
                                  VGint paramType,
                                  VGint count, const VGint * values)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGfloat vgGetParameterf(VGHandle object,
                                    VGint paramType)
{
	if (evgImageExists(object) == VG_TRUE)
	{
		//return evgImageGetParameteri((VGImage)object, paramType);
	}
	else if (evgPathExists(object) == VG_TRUE)
	{
		//return evgPathGetParameteri((VGPath)object, paramType);
	}
	else if (evgPaintExists(object) == VG_TRUE)
	{
		return evgPaintGetParameteri((VGPaint)object, paramType);
	}
	else
	{
		EVG_RET_ERROR(VG_BAD_HANDLE_ERROR, 0);
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGint vgGetParameteri(VGHandle object,
                                  VGint paramType)
{
	if (evgImageExists(object) == VG_TRUE)
	{
		return evgImageGetParameteri((VGImage)object, paramType);
	}
	else if (evgPathExists(object) == VG_TRUE)
	{
		return evgPathGetParameteri((VGPath)object, paramType);
	}
	else if (evgPaintExists(object) == VG_TRUE)
	{
		return evgPaintGetParameteri((VGPaint)object, paramType);
	}
	else
	{
		EVG_RET_ERROR(VG_BAD_HANDLE_ERROR, 0);
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGint vgGetParameterVectorSize(VGHandle object,
                                           VGint paramType)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetParameterfv(VGHandle object,
                                  VGint paramType,
                                  VGint count, VGfloat * values)
{
	EVG_ERROR_IF((count < 0), VG_ILLEGAL_ARGUMENT_ERROR);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgGetParameteriv(VGHandle object,
                                  VGint paramType,
                                  VGint count, VGint * values)
{
	EVG_ERROR_IF((count < 0), VG_ILLEGAL_ARGUMENT_ERROR);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void vgSetPaint(VGPaint paint, VGbitfield paintModes)
{
}
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL VGPaint vgGetPaint(VGPaintMode paintMode)
{
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
#if 0
	/**********/
	/* common */
	/**********/
	/* common */
	/* setup default values */
	e->matrix_mode = VG_MATRIX_PATH_USER_TO_SURFACE;
	e->fill_rule = VG_EVEN_ODD;
	e->image_quality = VG_IMAGE_QUALITY_FASTER;
	e->rendering_quality = VG_RENDERING_QUALITY_BETTER;
	e->blend_mode = VG_BLEND_SRC_OVER;
	e->image_mode = VG_DRAW_IMAGE_NORMAL;
	/* no scissor rects */
	e->stroke_line_width = 1.0f;
	e->stroke_cap_style = VG_CAP_BUTT;
	e->stroke_join_style = VG_JOIN_MITER;
	e->stroke_mitter_limit = 4.0f;
	/* no dash pattern, no dash_phase */
	e->stroke_dash_phase_reset = VG_FALSE;
	/* tile fill color and clear color 0,0,0,0 */
	e->masking = VG_FALSE;
	e->scissoring = VG_FALSE;
	e->pixel_layout = VG_PIXEL_LAYOUT_UNKNOWN;
	e->filter_format_linear = VG_FALSE;
	e->filter_format_premultiplied = VG_FALSE;
	e->filter_channel_mask = (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA);

#endif
	/******************/
	/* implementation */
	/******************/
	/* setup hashes */
	e->path_hash = edata_hash_new(edata_direct_hash, edata_direct_compare);
	e->image_hash = edata_hash_new(edata_direct_hash, edata_direct_compare);
	e->paint_hash = edata_hash_new(edata_direct_hash, edata_direct_compare);
	/* setup the renderer */

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
/**
 * To be documented
 * FIXME: To be fixed
 */
VG_API_CALL void evgSetTargetSurface(VGImage i)
{
	//_context->tsurface = (Evg_Image *)i;
}
