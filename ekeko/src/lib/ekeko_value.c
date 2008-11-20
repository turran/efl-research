#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Ekeko_Value_Impl
{
	Ekeko_Value_Compare cmp;
	size_t size;
} Ekeko_Value_Impl;

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
int ekeko_value_register(const char *name, Ekeko_Value_Compare cmp, size_t size)
{
	Ekeko_Value_Impl *impl;
	
	impl = malloc(sizeof(Ekeko_Value_Impl));
	impl->cmp = cmp;
	impl->size = size;
	
	
	_value_impls++;
}

void ekeko_value_int_from(Ekeko_Value *v, int i)
{
	v->type = EKEKO_ATTRIBUTE_INT;
	v->v.i = i;
}

Eina_Bool ekeko_value_int_cmp(int a, int b)
{
	if (a != b)
		return EINA_TRUE;
	return EINA_FALSE;
}

void ekeko_value_rectangle_from(Ekeko_Value *v, Eina_Rectangle *r)
{
	v->type = EKEKO_ATTRIBUTE_RECTANGLE;
	v->v.r = *r;
}

void ekeko_value_rectangle_coords_from(Ekeko_Value *v, int x, int y, int w, int h)
{
	v->type = EKEKO_ATTRIBUTE_RECTANGLE;
	eina_rectangle_coords_from(&v->v.r, x, y, w, h);
}

Eina_Bool ekeko_value_rectangle_cmp(Eina_Rectangle *a, Eina_Rectangle *b)
{
	
}

void ekeko_value_string_from(Ekeko_Value *v, const char *string)
{
	
}

Eina_Bool ekeko_value_string_cmp(const char *a, const char *b)
{
	if (!strcmp(a, b))
		return EINA_FALSE;
	return EINA_TRUE;
}
#if 0
void ekeko_value_dump(Ekeko_Value *v)
{
	switch (v->type)
	{
	case EKEKO_ATTRIBUTE_INT:
		printf("")		
	}

}
#endif
