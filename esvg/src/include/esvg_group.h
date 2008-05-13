#ifndef ESVG_GROUP_H_
#define ESVG_GROUP_H_

#define ESVG_GROUP_CLASS       (esvg_group_class_get())
#define ESVG_GROUP(obj)       (ESVG_OBJECT_CAST((obj), ESVG_GROUP_CLASS, ESVG_Group))
#define ESVG_IS_GROUP(obj)    (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_GROUP_CLASS))

typedef struct _ESVG_Group ESVG_Group;

struct _ESVG_Group
{
	/* inheritance */
	ESVG_Container container;
};

EAPI Eobj_Class * esvg_group_class_get(void);


#endif /*ESVG_GROUP_H_*/
