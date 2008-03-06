#ifndef ETCH_PRIVATE_H_
#define ETCH_PRIVATE_H_

#include <stdlib.h>

/**
 * 
 */
struct _Etch
{
	/* list of objects? */
};

/**
 * 
 */
struct _Etch_Object
{
	const char *id; /** A way to identify the object */
	Etch_Object_Class *oclass; /** Object Class */
	void *pros; /** Where all the properties are saved */
	int *offsets; /* for each property on the object class we store here the offset */
};

#endif /*ETCH_PRIVATE_H_*/
