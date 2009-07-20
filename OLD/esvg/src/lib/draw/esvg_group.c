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
static void _constructor(ESVG_Group *o)
{
	
}
static void _destructor(ESVG_Group *o)
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
EAPI Eobj_Class * esvg_group_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		eobj_class_new("ESVG_Group", ESVG_CONTAINER_CLASS,
			sizeof(ESVG_Group), EOBJ_CONSTRUCTOR(_constructor),
			EOBJ_DESTRUCTOR(_destructor), NULL);
	}
	return c;
}
