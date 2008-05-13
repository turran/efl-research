#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _constructor(ESVG_Container *o)
{
	
}
static void _destructor(ESVG_Container *o)
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
EAPI Eobj_Class * esvg_container_class_get(void)
{
	static Eobj_Class *c = NULL;
		
	if (!c)
	{
		eobj_class_new("ESVG_Container", ESVG_ELEMENT_CLASS,
			sizeof(ESVG_Container), EOBJ_CONSTRUCTOR(_constructor),
			EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;	
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_container_add(ESVG_Container *c, ESVG_Element *e)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI void esvg_container_remove(ESVG_Element *e)
{
	
}
/* container types:
 * svg
 * group
 * defs
 * symbol
 * clippath
 * mask
 * pattern
 * marker
 * a
 * switch
 */
