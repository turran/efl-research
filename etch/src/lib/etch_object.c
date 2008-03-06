#include "Etch.h"
#include "etch_private.h"

/* TODO
 * do a list of types, size in bytes, and alignement in case it needs we need
 * like:
 * UINT32, 4, 4
 */
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * Adds an object to an Etch
 * e: Etch to add this object to
 * oc: object's class
 * id: the unique object's id 
 * data: be passed to every property set function 
 */
EAPI Etch_Object * etch_object_add(Etch *e, Etch_Object_Class *oc, const char *id, void *data)
{
	Etch_Object *o;
	Etch_Object_Property *p;
	int i = 0;
	
	o = malloc(sizeof(Etch_Object));
	
	/* FIXME count the number of properties, maybe the class could have a variable
	 * called props_num */
	p = oc->props;
	while (p->set)
	{
		i++;
		p++;
	}
	/* get the offsets for each element */
	o->offsets = malloc(sizeof(int) * i);
	p = oc->props;
	while (p->set)
	{
		int type;
		
		/* get the type and incrment the offset */
		type = ETCH_PROPERTY_DATATYPE_GET(p->type);
		switch (type)
		{
			case ETCH_UINT32:
			case ETCH_INT32:
			case ETCH_FLOAT:
			case ETCH_DOUBLE:
			case ETCH_ARGB:
				/* TODO add the correct offset */
				break;
			default:
				/* bad type, not supported class */
				break;
				
		}
		p++;
	}
}

/**
 * 
 */
EAPI Etch_Object * etch_object_get_by_id(Etch *e, const char *id)
{
	/* from the list of objects in e, iterate and return the matching
	 * object
	 */
}

/**
 * 
 */
EAPI void * etch_object_property_get(Etch_Object *eo, int type)
{
	/* search in the description the index i of the matching property */
	/* return (char *)eo->props + eo->offsets[i] */ 
}

/**
 * sets a value into the specified
 */
EAPI void * etch_object_property_set(Etch_Object *eo, int prop, void *data)
{
	/* same as the get function but set the value with the data passed */
}
