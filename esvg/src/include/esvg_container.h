#ifndef ESVG_CONTAINER_H_
#define ESVG_CONTAINER_H_

#define ESVG_CONTAINER_CLASS       (esvg_container_class_get())
#define ESVG_CONTAINER(obj)       (ESVG_OBJECT_CAST((obj), ESVG_CONTAINER_CLASS, ESVG_Container))
#define ESVG_IS_CONTAINER(obj)    (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_CONTAINER_CLASS))

typedef struct _ESVG_Container ESVG_Container;

struct _ESVG_Container
{
	/* private */
	/* inheritance */
	ESVG_Element element;
	void (*child_add)(ESVG_Container *container, ESVG_Element *element);
	void (*child_remove)(ESVG_Container *container, ESVG_Element *element);
};

EAPI Eobj_Class * esvg_container_class_get(void);
EAPI void esvg_container_add(ESVG_Container *c, ESVG_Element *e);
EAPI void esvg_container_remove(ESVG_Element *e);

#endif /*ESVG_CONTAINER_H_*/
