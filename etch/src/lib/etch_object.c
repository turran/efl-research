#include "Etch.h"
#include "etch_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* this lsit specifies the length and alignment of each data type, keeps the
 * same order as the enum on Etch.h
 */
static int _data_info[][2] = {
	{4, 4}, /* ETCH_UINT32 */
	{4, 4}, /* ETCH_INT32 */
	{4, 4}, /* ETCH_FLOAT */
	{8, 8}, /* ETCH_DOUBLE */
	{4, 4}, /* ETCH_ARGB */
};

#define DATA_LENGTH 0
#define DATA_ALIGNMENT 1

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

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
	int length = 0;
	
	o = malloc(sizeof(Etch_Object));
	
	/* TODO maybe the class could have a variable called props_num ? */
	/* count the number of properties */
	p = oc->props;
	while (p->set)
	{
		i++;
		p++;
	}
	/* get the offsets for each element */
	o->offsets = malloc(sizeof(int) * i);
	p = oc->props;
	i = 0;
	while (p->set)
	{
		int type;
		
		/* get the type and incrment the offset */
		type = ETCH_PROPERTY_DATATYPE_GET(p->type);
		/* add the correct offset */
		length += _data_info[type][DATA_LENGTH];
		/* TODO check the alignment */
		o->offsets[i] = length;
		p++;
		i++;
	}
	/* allocate the properties */
	o->props = malloc(length);
	/* initialize the animations */
	o->animations = calloc(i, sizeof(Etch_Animation *));
	/* setup the common object values */
	o->id = strdup(id);
	o->oclass = oc;
	o->etch = e;
	o->data = data;
	o->nprops = i;
	
	/* append the object to the list of objects */
	e->objects = eina_inlist_append(e->objects, o);
	
	return o;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_object_delete(Etch_Object *o)
{
	o->etch->objects = eina_inlist_remove(o->etch->objects, o);
	free(o->offsets);
	free(o->props);
	free(o);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Etch_Object * etch_object_get_by_id(Etch *e, const char *id)
{
	/* from the list of objects in e, iterate and return the matching
	 * object
	 */
}

/* TODO split the get/set into a common function */ 

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_object_property_get(Etch_Object *eo, int prop, void *data)
{
	Etch_Object_Property *p;
	int i = 0;
	int dtype;
	
	/* search in the description the index of the matching property */
	p = eo->oclass->props;
	while (p->set)
	{
		if (ETCH_PROPERTY_TYPE_GET(p->type) == prop)
		{
			dtype = ETCH_PROPERTY_DATATYPE_GET(p->type);
			goto ok;
		}
		i++;
		p++;
	}
	return;
ok:
	memcpy(data, (void *)((char *)eo->props + eo->offsets[i]), _data_info[dtype][DATA_LENGTH]); 
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_object_property_set(Etch_Object *eo, int prop, void *data)
{
	Etch_Object_Property *p;
	int i = 0;
	int dtype;
	
	/* search in the description the index of the matching property */
	p = eo->oclass->props;
	while (p->set)
	{
		if (ETCH_PROPERTY_TYPE_GET(p->type) == prop)
		{
			dtype = ETCH_PROPERTY_DATATYPE_GET(p->type);
			goto ok;
		}
		i++;
		p++;
	}
	return;
ok:
	memcpy((void *)((char *)eo->props + eo->offsets[i]), data, _data_info[dtype][DATA_LENGTH]);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_object_animation_set(Etch_Object *eo, int prop, Etch_Animation *a)
{
	/* check that the data type of the property is the same */
	if (ETCH_PROPERTY_DATATYPE_GET(prop) != a->dtype)
		return;
	/* check if there's already an animation for this property
	 * if so replace it
	 */
	eo->animations[ETCH_PROPERTY_TYPE_GET(prop)] = a;
}
