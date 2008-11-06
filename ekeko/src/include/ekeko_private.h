#ifndef EKEKO_PRIVATE_H_
#define EKEKO_PRIVATE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "config.h"

#define DEBUG
/* Magic values for each system */
typedef enum
{
	EKEKO_CANVAS_MAGIC  = 0x20000000,
	EKEKO_OBJECT_MAGIC = 0x21000000,
} Ekeko_Magic;

/* Debugging routines
 * ASSERT will abort the program execution
 * ERROR will set the error and return false
 */
#ifndef DEBUG
#define EKEKO_MAGIC_CHECK(p, m)
#define EKEKO_MAGIC_SET(p, m)
#define EKEKO_ASSERT(cond, err)
#define EKEKO_ERROR(err) \
	ekeko_error_set(err); \
	return EINA_FALSE; \
#define EKEKO_ERROR_COND(err, cond) \
	if (!(cond)) \
	{ \
		EKEKO_ERROR(err);
	}
#else

#define EKEKO_MAGIC_SET(p, m) \
	p->magic = m;
#define EKEKO_MAGIC_CHECK(p, m) \
	if (p->magic != m) \
	{ \
		fprintf(stderr, "[Ekeko] Magic Failed. %s at %s:%d - %s():\n", ekeko_error_to_str(EKEKO_ERROR_HANDLE_INVALID), __FILE__, __LINE__, __FUNCTION__); \
		abort(); \
	}

#define EKEKO_ERROR(err) \
	fprintf(stderr, "[Ekeko] %s at %s:%d - %s():\n", ekeko_error_to_str(err), __FILE__, __LINE__, __FUNCTION__); \
	ekeko_error_set(err); \
	return EINA_FALSE; \

#define EKEKO_ERROR_COND(err, cond) \
	if (!(cond)) \
	{ \
		EKEKO_ERROR(err) \
	}
#define EKEKO_ASSERT(cond, err) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "[Ekeko] %s at %s:%d - %s():\n", ekeko_error_to_str(err), __FILE__, __LINE__, __FUNCTION__); \
		abort(); \
	}
#endif


#define EKEKO_EVENT

#include "private/attribute.h"
#include "private/document.h"
#include "private/element.h"
#include "private/input.h"
#include "private/tiler.h"
#include "private/renderable.h"
#include "private/canvas.h"

#endif /*EKEKO_PRIVATE_H_*/
