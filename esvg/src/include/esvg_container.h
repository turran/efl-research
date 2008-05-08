#ifndef ESVG_CONTAINER_H_
#define ESVG_CONTAINER_H_

typedef struct _ESVG_Container ESVG_Container;

struct _ESVG_Container
{
	/* private */
	/* inheritance */
	ESVG_Element element;
};

EAPI void esvg_container_add(ESVG_Container *c, ESVG_Element *e);
EAPI void esvg_container_remove(ESVG_Element *e);

#endif /*ESVG_CONTAINER_H_*/
