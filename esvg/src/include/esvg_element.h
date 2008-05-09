#ifndef ESVG_ELEMENT_H_
#define ESVG_ELEMENT_H_

/**
 * Base type
 */


#define ESVG_ELEMENT_CLASS       (esvg_element_class_get())
#define ESVG_ELEMENT(obj)       (ESVG_OBJECT_CAST((obj), ESVG_ELEMENT_CLASS, ESVG_Element))
#define ESVG_IS_ELEMENT(obj)    (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_ELEMENT_CLASS))

typedef struct _ESVG_Element ESVG_Element;

struct _ESVG_Element
{
	/* inheritance */
	ESVG_Object object;
	/* attributes */
	ESVG_Element *ancestor;
	ESVG_Attribute_Core core;
};

EAPI void esvg_element_id_set(ESVG_Element *e, const char *id);
EAPI ESVG_Element * esvg_element_ancestor_get(ESVG_Element *e);
EAPI ESVG_Class *esvg_element_class_get(void);

#endif /*ESVG_ELEMENT_H_*/
