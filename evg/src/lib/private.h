#ifndef _PRIVATE_H
#define _PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include "Enesim.h"
#include "Edata.h"

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

VGboolean evgPathExists(VGPath p);
void evgSetError(VGErrorCode err);
void evgCreatePath(VGPath p);

#endif
