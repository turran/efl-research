#ifndef ETCH_PRIVATE_H_
#define ETCH_PRIVATE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdarg.h>
#include <math.h> 
#include <sys/time.h>

#include <assert.h>

#include "Eina.h"

/**
 * Container of every property data type supported
 */
typedef struct _Etch_Data
{
	unsigned int u32;
	int i32;
	float f;
	double d;
	unsigned int argb;	
} Etch_Data;


/**
 * 
 */
struct _Etch
{
	Eina_Inlist *objects; /** List of objects */
	unsigned long frame; /** Current frame */
	unsigned int fps; /** Number of frames per second */
	double curr; /** Current time in seconds */
	double start; /** Time where an animation starts in seconds */
	double end; /** Time where an animation ends in seconds */	
};

/**
 * 
 */
struct _Etch_Object
{
	EINA_INLIST; /** Internal list */ 
	Etch *etch; /** Etch container */
	const char *id; /** A way to identify the object */
	Etch_Object_Class *oclass; /** Object Class */
	void *props; /** Where all the properties are saved */
	int *offsets; /** For each property on the object class we store here the offset */
	void *data; /** User defined data */
	int nprops; /** Number of properties */
	Etch_Animation **animations; /** Array of animation per property */
};

/**
 * Specific data needed for cubic bezier animations
 */
typedef struct _Etch_Animation_Cubic
{
	Etch_Data cp1; /** First control point */
	Etch_Data cp2; /** Second control point */
} Etch_Animation_Cubic;

/**
 * Specific data needed for quadratic bezier animations
 */
typedef struct _Etch_Animation_Quadratic
{
	Etch_Data cp; /** Control point */ 
} Etch_Animation_Quadratic;

/**
 * An animation mark is a defined state on the timeline of an animation. It sets
 * that a given time a property should have the specified value.
 */
struct _Etch_Animation_Keyframe
{
	EINA_INLIST; /** A keyframe is always a list */
	Etch_Animation *animation; /** reference to the animation */
	Etch_Data value; /** the property value for this mark */
	double time; /** the time where the keyframe is, already transformed to seconds */
	Etch_Animation_Type type; /** type of interpolation between this mark and the next */
	union {
		Etch_Animation_Quadratic q;
		Etch_Animation_Cubic c;
	} data; /** interpolation specific data */ 
};

/**
 * Many objects can use the same animation.
 */
struct _Etch_Animation
{
	Eina_Inlist *keys; /* list of keyframes */
	/* TODO if the marks are already ordered do we need to have the start
	 * and end time duplicated here? */
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
