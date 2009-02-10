/*
 * etk_private.h
 *
 *  Created on: 14-Dec-2008
 *      Author: Hisham Mardam-Bey <hisham.mardambey@gmail.com>
 */

#ifndef EKEKO_PRIVATE_H
#define EKEKO_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define EKEKO_DEBUG

#define RETURN_IF(expr) if ((expr)) return
#define RETURN_NULL_IF(expr) if ((expr)) return NULL

#define OFFSET(type, mem) ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

#include <private/object.h>
#include <private/renderable.h>
#include <private/input.h>
#include <private/type.h>
#include <private/value.h>
#include <private/event.h>
#include <private/property.h>

#endif
