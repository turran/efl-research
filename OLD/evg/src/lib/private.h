#ifndef _PRIVATE_H
#define _PRIVATE_H

#include <stdio.h>
#include <stdlib.h>

#include "Eina.h"
#include "Enesim.h"



/**
 * @todo
 * - context (WIP)
 * - setters/getters
 * - rendering quality/antialiasing
 * - scissoring/masking/clearing
 * - paths (WIP)
 * - paint
 * - image
 * - filters
 * - blending
 */

#define EVG_RET_ERROR_IF(cond, err, ret) \
	if (cond)                            \
	{                                    \
		evgSetError(err);                \
		return ret;                      \
	}

#define EVG_ERROR_IF(cond, err) \
	if (cond)                   \
	{                           \
		evgSetError(err);       \
		return;                 \
	}

#define EVG_RET_ERROR(err, ret) \
	evgSetError(err);           \
	return ret;


typedef struct _Evg_Image Evg_Image;

VGboolean evgImageExists(VGImage i);
VGboolean evgPaintExists(VGPaint p);
VGboolean evgPathExists(VGPath p);

void evgCreatePath(VGPath p);
void evgCreatePaint(VGPaint p);
void evgCreateImage(VGImage i);

void evgSetError(VGErrorCode err);

VGint evgPaintGetParameteri(VGPaint p, VGint paramType);
VGint evgPathGetParameteri(VGPath p, VGint paramType);
VGint evgImageGetParameteri(VGImage i, VGint paramType);

#endif
