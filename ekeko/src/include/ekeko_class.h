#ifndef EKEKO_CLASS_H_
#define EKEKO_CLASS_H_

typedef struct _Ekeko_Class_Description
{
	/* common description */
	const char *namespace;
        const char *name;
        int size;
        int priv_size;
        /* mandatory functions */
        void (*init)(Ekeko_Class *, Ekeko_Element *);
        void (*shutdown)(Ekeko_Element *);
        Eina_Bool (*appendable)(Ekeko_Element *, Ekeko_Element *, Ekeko_Exception *ex);
        Eina_Bool (*attribute_set)(Ekeko_Element *e, const char *attr,
        		Ekeko_Value *val, Ekeko_Exception *ex);
        Eina_Bool (*attribute_get)(Ekeko_Element *e, const char *attr,
        		Ekeko_Value *val, Ekeko_Exception *ex);
} Ekeko_Class_Description;

EAPI Ekeko_Class * edom_class_new(Ekeko_Class_Description *cd, Ekeko_Class *c);

#endif /*EKEKO_CLASS_H_*/
