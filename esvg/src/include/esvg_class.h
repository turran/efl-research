#ifndef ESVG_CLASS_H_
#define ESVG_CLASS_H_

/**
 * TODO This OO approach can be part of eina??
 */

#define ESVG_OBJECT_CAST(obj, class, c_type)     ((c_type *)(obj))
#define ESVG_OBJECT_CHECK_CLASS(obj, class) \
   (esvg_class_inherits_from(((ESVG_Object *)(obj))->class, (class)))

#define ESVG_OBJECT_CLASS (esvg_object_class_get())
#define ESVG_OBJECT(obj) (ESVG_OBJECT_CAST((obj), ESVG_OBJECT_CLASS, ESVG_Object))
#define ESVG_IS_OBJECT(obj) (ESVG_OBJECT_CHECK_CLASS((obj), ESVG_OBJECT_CLASS))

typedef struct _ESVG_Class ESVG_Class;
typedef struct _ESVG_Object ESVG_Object;

typedef void (*ESVG_Class_Constructor)(ESVG_Object *e);
typedef void (*ESVG_Class_Destructor)(ESVG_Object *e);

struct _ESVG_Object
{
	ESVG_Class *class;
};

struct _ESVG_Class
{
	char *name;
	unsigned int size;
	ESVG_Class *parent;
	ESVG_Class_Constructor constructor;
	ESVG_Class_Destructor destructor;
};

EAPI ESVG_Class * esvg_class_new(const char *class_name, ESVG_Class *parent,
		unsigned int size, ESVG_Class_Constructor constructor,
		ESVG_Class_Destructor destructor);
EAPI Eina_Bool esvg_class_inherits_from(ESVG_Class *c, ESVG_Class *parent);

#endif /*ESVG_CLASS_H_*/
