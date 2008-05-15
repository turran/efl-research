#ifndef ESVG_ELEMENT_H_
#define ESVG_ELEMENT_H_

/**
 * Base type
 */


#define ESVG_ELEMENT_CLASS       (esvg_element_class_get())
#define ESVG_ELEMENT(obj)       (EOBJ_OBJECT_CAST((obj), ESVG_ELEMENT_CLASS, ESVG_Element))
#define ESVG_IS_ELEMENT(obj)    (EOBJ_OBJECT_CHECK_CLASS((obj), ESVG_ELEMENT_CLASS))

struct _ESVG_Element
{
	/* inheritance */
	Eobj_Object object;
	/* attributes */
	ESVG_Svg *ancestor;
	ESVG_Element *viewport; /* the parent element that stablish a new viewport */
	ESVG_Container *parent;
	ESVG_Attribute_Core core;
	/* TODO style attributes */
};

EAPI Eobj_Class * esvg_element_class_get(void);
EAPI void esvg_element_id_set(ESVG_Element *e, const char *id);
EAPI ESVG_Svg * esvg_element_ancestor_get(ESVG_Element *e);
EAPI ESVG_Element * esvg_element_new(Eobj_Class *class, const char *first_property, ...);

#endif /*ESVG_ELEMENT_H_*/
