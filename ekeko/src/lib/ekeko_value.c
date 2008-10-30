#include "Edom.h"
#include "edom_private.h"

#include <string.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Edom_Value_Impl
{
	Edom_Value_Compare cmp;
	size_t size;
} Edom_Value_Impl;

static int _value_impls = 0;

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Register a new value type
 * the compare function to determine if it has changed
 */
int edom_value_register(const char *name, Edom_Value_Compare cmp, size_t size)
{
	Edom_Value_Impl *impl;
	
	impl = malloc(sizeof(Edom_Value_Impl));
	impl->cmp = cmp;
	impl->size = size;
	
	
	_value_impls++;
}

void edom_value_int_from(Edom_Value *v, int i)
{
	v->type = EDOM_ATTRIBUTE_INT;
	v->v.i = i;
}

Eina_Bool edom_value_int_cmp(int a, int b)
{
	if (a != b)
		return EINA_TRUE;
	return EINA_FALSE;
}

void edom_value_rectangle_from(Edom_Value *v, Eina_Rectangle *r)
{
	v->type = EDOM_ATTRIBUTE_RECTANGLE;
	v->v.r = *r;
}

Eina_Bool edom_value_rectangle_cmp(Eina_Rectangle *a, Eina_Rectangle *b)
{
	
}

void edom_value_string_from(Edom_Value *v, const char *string)
{
	
}

Eina_Bool edom_value_string_cmp(const char *a, const char *b)
{
	if (!strcmp(a, b))
		return EINA_FALSE;
	return EINA_TRUE;
}

