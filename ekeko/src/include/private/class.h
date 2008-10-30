#ifndef CLASS_H_
#define CLASS_H_

/* TODO add functions for node adding / removing / cloning, etc */
struct _Edom_Class
{
	const char *namespace;
	const char *name;
	Edom_Class **classes;
	int num_classes;
	int priv_size;
	int total_size;
        void (*init)(Edom_Class *, Edom_Element *);
	void (*shutdown)(Edom_Element *);
	Eina_Bool (*appendable)(Edom_Element *, Edom_Element *, Edom_Exception *ex);
	Eina_Bool (*attribute_set)(Edom_Element *e, const char *attr, Edom_Value *val, Edom_Exception *ex);
	Eina_Bool (*attribute_get)(Edom_Element *e, const char *attr, Edom_Value *val, Edom_Exception *ex);
};

Edom_Element *edom_class_init(Edom_Class *c);

#endif /*CLASS_H_*/
