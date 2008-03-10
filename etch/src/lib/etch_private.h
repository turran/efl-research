#ifndef ETCH_PRIVATE_H_
#define ETCH_PRIVATE_H_

#include <stdlib.h>
#include <string.h>

/**
 * 
 */
struct _Etch
{
	/* list of objects? */
	
	/* how to store the time ? better count frames right?
	 * int (2^32) =  4 294 967 296 frames / fps (30) = 143 165 577 s
	 * ~39 768 hours, i guess is enough; but if we use milliseconds ?
	 */
	unsigned long frame; /** current frame */
	unsigned int fps; /** number of frames per second */
	unsigned long sec_start; /** initial second where an animation starts */
	unsigned long sec_end; /** last second where all anymations end */ 
	/* we will also need function to set the time then, so we can forward,
	 * rewind, restart, etc */
	
};

/**
 * 
 */
struct _Etch_Object
{
	Etch *etch; /** Etch container */
	const char *id; /** A way to identify the object */
	Etch_Object_Class *oclass; /** Object Class */
	void *props; /** Where all the properties are saved */
	int *offsets; /** For each property on the object class we store here the offset */
	void *data; /** User defined data */ 
	/* TODO place here an animation for each property? */
};


/**
 * An animation mark is a defined state on the timeline of an animation. It sets
 * that a given time a property should have the specified value.
 */
typedef struct _Etch_Animation_Mark
{
	void *value; /** the property value for this mark */
	unsigned long time; /* what units use here? ms or s? */
	/* TODO the way to interpolate between this mark and the next */
} Etch_Animation_Mark;

/**
 * 
 */
typedef struct _Etch_Animation
{
	Etch_Animation_Mark *marks;
	unsigned long start; /** initial time */
	unsigned long end; /** end time */
	/* TODO we need to reference the property, do it here or in the object itself? */
} Etch_Animation;

#endif /*ETCH_PRIVATE_H_*/
