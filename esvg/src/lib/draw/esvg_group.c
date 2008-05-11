#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* Container */
static void _child_add(ESVG_Container *container, ESVG_Element *element)
{
	
}
static void _child_remove(ESVG_Container *container, ESVG_Element *element)
{
	
}
/* Class */
static void _constructor(ESVG_Object *o)
{
	
}
static void _destructor(ESVG_Object *o)
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
ESVG_Class * esvg_group_class_get(void)
{
	static ESVG_Class *c = NULL;
	
	if (!c)
	{
		esvg_class_new("ESVG_Group", ESVG_CONTAINER_CLASS,
			sizeof(ESVG_Group), _constructor, _destructor);
	}
	return c;
}
