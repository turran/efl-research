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
ESVG_Class * esvg_container_class_get(void)
{
	static ESVG_Class *c = NULL;
		
	if (!c)
	{
		esvg_class_new("ESVG_Container", ESVG_ELEMENT_CLASS,
			sizeof(ESVG_Container), ESVG_CONSTRUCTOR(_constructor),
			ESVG_DESTRUCTOR(_destructor));
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
