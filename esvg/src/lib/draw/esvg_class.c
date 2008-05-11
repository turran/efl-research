#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _object_constructor(ESVG_Object *o)
{
	
}
static void _object_destructor(ESVG_Object *o)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI ESVG_Class * esvg_class_new(const char *class_name, ESVG_Class *parent,
		unsigned int size, ESVG_Class_Constructor constructor,
		ESVG_Class_Destructor destructor)
{
	ESVG_Class *c;
	
	if (!class_name)
		return NULL;
	
	c = malloc(sizeof(ESVG_Class));
	c->constructor = constructor;
	c->destructor = destructor;
	c->size = size;
	
	if (parent)
	{
		c->parent = parent;
	}
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI Eina_Bool esvg_class_inherits_from(ESVG_Class *c, ESVG_Class *parent)
{
	ESVG_Class *cparent;
	
	if (!c || !parent)
		return EINA_FALSE;
	if (c == parent)
		return EINA_TRUE;
	
	cparent = c->parent;
	while (cparent)
	{
		if (parent == cparent)
			return EINA_TRUE;
		cparent = cparent->parent;
	}
	return EINA_FALSE;
}

EAPI ESVG_Class * esvg_object_class_get(void)
{
	static ESVG_Class *c = NULL;
	
	if (!c)
	{
		
	}
	return c;
}
