#include "Etch.h"
#include "etch_private.h"

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
	a->dtype = dtype;
	
	return a;
}

EAPI void etch_animation_free(Etch_Animation *a)
{
	/* TODO delete the list of marks */
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
	/* TODO check the keyframe's time, and place it like start or end if necessary */
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
	struct timeval t;
	
	t.tv_sec = secs;
	t.tv_usec = usecs;
	m->time = etch_timeval_to_double(&t);
	
	/* TODO reorder the list of keyframes based on its time */
}

/**
 * Set the value for a mark
 */
EAPI void etch_animation_keyframe_value_set(Etch_Animation_Keyframe *m, int type, ...)
{
	
}
