#ifndef ELEMENT_H_
#define ELEMENT_H_


struct _Edom_Element_Private
{
	Eina_Inlist _list;
	Eina_Hash *attributes;
	Edom_Element *parent;
        Eina_List *children;
        Edom_Class *clss;
        //Edom_Element *referenced; /* edom_elements that use this as a reference */
};

Edom_Class * edom_element_class(void);

#endif /*ELEMENT_H_*/
