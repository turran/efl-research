#include "Etch.h"
#include "etch_private.h"

/* REMOVE THIS */
Etch_Animation_Keyframe *k1 = NULL;
Etch_Animation_Keyframe *k2 = NULL;

/**
 * TODO
 * + make functions to interpolate between data types,
 * possible animations:
 * sin
 * exp
 * log
 * linear
 * bezier based (1 and 2 control points)
 * + make every interpolator work for every data type, so better a function table
 * + define animatinos based on two properties: PERIODIC, UNIQUE, PERIODIC_MIRROR
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _linear_uint32(void *data, double m, unsigned int a, unsigned int b, void *res)
{
	double r;
	
	/* handle specific case where a and b are equal (constant) */
	if (a == b)
		*(unsigned int *)res = a;
	r = (double)a * (1 - m) + (double)b * m;
	*(unsigned int *)res = r;
}

static void _cosin_uint32(void *data, double m, unsigned int a, unsigned int b, void *res)
{
	/* double m2;
	 * m2 = (1-cos(m*PI))/2;
	 * return(a*(1-m2)+b*m2);
	 * where m = [0, 1]
	 */
}

static void _bquad_uint32(void *data, double m, unsigned int a, unsigned int b, void *res)
{
	/* 
	 * p = bezier control point
	 * return (1-m)²a + 2m(1 - m)p + t²b
	 * where m = [0, 1]
	 */
}

static void _bcubic_uint32(void *data, double m, unsigned int a, unsigned int b, void *res)
{
	/* 
	 */
}
/* prototype of the function table */
typedef void (*Etch_Interpolator)(void *data, double m, void *a, void *b, void *res);

Etch_Interpolator _interpolators[ETCH_ANIMATIONS][ETCH_DATATYPES] = {
		[ETCH_ANIMATION_LINEAR][ETCH_UINT32] = (Etch_Interpolator)_linear_uint32,
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * 
 */
void etch_animation_object_animate(Etch_Animation *a, Etch_Object *o, double curr)
{
	/* TODO instead of checking everytime every keyframe we can translate the
	 * keyframes based on the frame, when a keyframe has passed move it before
	 * like a circular list */
	
	/* check that the time is between two keyframes */
	/* REMOVE THIS, we are assuming that k2 is after k1, only for testing */
	if ((curr >= k1->time) && (curr <= k2->time))
	{
		double m;
			
		/* get the interval between 0 and 1 based on current frame and two keyframes */
		m = (curr - k1->time)/(k2->time - k1->time);
		/* accelerate the calculations if we get the same m as the previous call */
		if (m == a->m)
		{
			/* TODO instead of continue call the set callback again */
			return;
		}
		/* calculate the appropiate property and call the property set function */
		printf("inside\n");
	}
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * TODO: define what kind of property/value this will animate?
 */
EAPI Etch_Animation * etch_animation_new(int dtype)
{
	Etch_Animation *a;
	
	a = calloc(1, sizeof(Etch_Animation));
	/* common values */
	a->m = -1; /* impossible, so the first keyframe will overwrite this */
	a->start = DBL_MAX;
	a->dtype = dtype;
	
	return a;
}

EAPI void etch_animation_free(Etch_Animation *a)
{
	/* TODO delete the list of keyframes */
	free(a);
}

/**
 * Add a new mark to the animation
 */
EAPI Etch_Animation_Keyframe * etch_animation_keyframe_add(Etch_Animation *a)
{
	Etch_Animation_Keyframe *k;
	
	k = calloc(1, sizeof(Etch_Animation_Keyframe));
	k->animation = a;

	/* REMOVE THIS */
	if (!k1)
		k1 = k;
	else if (!k2)
		k2 = k;
	
	return k;
}

/**
 * Delete the mark from the animation
 */
EAPI void etch_animation_keyframe_del(Etch_Animation *a, Etch_Animation_Keyframe *m)
{
	/* remove the mark from the list and recalculate the start and end if necessary */
}

/* TODO do we need to be this finegrained? */

/**
 * Set the time for a mark
 */
EAPI void etch_animation_keyframe_time_set(Etch_Animation_Keyframe *m, unsigned long secs, unsigned long usecs)
{
	Etch_Animation *a;
	struct timeval t;
	
	t.tv_sec = secs;
	t.tv_usec = usecs;
	m->time = etch_timeval_to_double(&t);
	a = m->animation;
	
	/* update the start and end values */
	if (a->start > m->time)
	{
		a->start = m->time;
	}
	if (m->time > a->end)
	{
		a->end = m->time;
	}
	/* TODO reorder the list of keyframes based on its time */
}

/**
 * Set the value for a mark
 */
EAPI void etch_animation_keyframe_value_set(Etch_Animation_Keyframe *m, int type, ...)
{
	/* TODO handle vargs */
}
