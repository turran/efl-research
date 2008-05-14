#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _constructor(ESVG_Element *e)
{
	printf("called constructor\n");
}
static void _destructor(ESVG_Element *e)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/* function to get the ekeko canvas (parent SVG) */
/* function to get the topmost ekeko_canvas */
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed 
 */
EAPI Eobj_Class *esvg_element_class_get(void)
{
	static Eobj_Class *c = NULL;
	
	if (!c)
	{
		c = eobj_class_new("ESVG_Element", EOBJ_OBJECT_CLASS,
			sizeof(ESVG_Element), EOBJ_CONSTRUCTOR(_constructor),
			EOBJ_DESTRUCTOR(_destructor), NULL);
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
EAPI ESVG_Element * esvg_element_new(Eobj_Class *class, const char *first_property, ...)
{
	ESVG_Element *new_element;
	va_list args;

	if (!class)
		return NULL;

	va_start(args, first_property);
	new_element = ESVG_ELEMENT(eobj_object_new_valist(class, first_property, args));
	va_end(args);

	return new_element;
}
