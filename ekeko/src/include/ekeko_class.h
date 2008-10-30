#ifndef EKEKO_CLASS_H_
#define EKEKO_CLASS_H_

typedef struct _Edom_Class_Description
{
	/* common description */
	const char *namespace;
        const char *name;
        int size;
        int priv_size;
        /* mandatory functions */
        void (*init)(Edom_Class *, Edom_Element *);
        void (*shutdown)(Edom_Element *);
        Eina_Bool (*appendable)(Edom_Element *, Edom_Element *, Edom_Exception *ex);
        Eina_Bool (*attribute_set)(Edom_Element *e, const char *attr,
        		Edom_Value *val, Edom_Exception *ex);
        Eina_Bool (*attribute_get)(Edom_Element *e, const char *attr,
        		Edom_Value *val, Edom_Exception *ex);
} Edom_Class_Description;

EAPI Edom_Class * edom_class_new(Edom_Class_Description *cd, Edom_Class *c);

#endif /*EKEKO_CLASS_H_*/
