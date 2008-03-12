#ifndef ETCH_PRIVATE_H_
#define ETCH_PRIVATE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdarg.h>
#include <math.h> 
#include <sys/time.h>

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
	double curr; /** current time in seconds */
	double start; /** time where an animation starts in seconds */
	double end; /** time where an animation ends in seconds */	
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
	int nprops; /** number of properties */
	Etch_Animation **animations; /** List of animation per property */
};

/**
 * Specific data needed for cubic bezier animations
 */
typedef struct _Etch_Animation_Cubic
{
	void *cp1;
	void *cp2;
} Etch_Animation_Cubic;

/**
 * Specific data needed for quadratic bezier animations
 */
typedef struct _Etch_Animation_Quadratic
{
	void *cp1;
} Etch_Animation_Quadratic;

/**
 * An animation mark is a defined state on the timeline of an animation. It sets
 * that a given time a property should have the specified value.
 */
struct _Etch_Animation_Keyframe
{
	Etch_Animation *animation; /** reference to the animation */
	void *value; /** the property value for this mark */
	double time; /** the time where the keyframe is, already transformed to seconds */
	int type; /** type of interpolation between this mark and the next */
	void *data; /** interpolation specific data */
};

/**
 * Many objects can use the same animation.
 */
struct _Etch_Animation
{
	Etch_Animation_Keyframe *marks;
	/* TODO if the marks are already ordered do we need to have the start
	 * and end time duplicated here */
	double start; /** initial time already transformed to seconds */
	double end; /** end time already transformed to seconds */
	double m; /** last interpolator value in the range [0,1] */
	int dtype; /** animations only animates data types, no properties */
};

static inline double etch_timeval_to_double(struct timeval *t)
{
	return (double)t->tv_sec + (((double)t->tv_usec) / 1000000);
}

void etch_animation_data_animate(Etch_Animation *a, void *pdata, double curr);

#endif /*ETCH_PRIVATE_H_*/
