#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _constructor(ESVG_Element *e)
{
	
}
static void _destructor(ESVG_Element *e)
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
EAPI ESVG_Class *esvg_element_class_get(void)
{
	static ESVG_Class *c = NULL;
	
	if (!c)
	{
		c = esvg_class_new("ESVG_Element", ESVG_OBJECT_CLASS,
			sizeof(ESVG_Element), ESVG_CONSTRUCTOR(_constructor),
			ESVG_DESTRUCTOR(_destructor));
	}
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_element_id_set(ESVG_Element *e, const char *id)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI ESVG_Element * esvg_element_ancestor_get(ESVG_Element *e)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI ESVG_Element * esvg_element_new(ESVG_Class *class, const char *first_property, ...)
{
	
}
