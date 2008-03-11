#include "Etch.h"
#include "etch_private.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI Etch_Animation * etch_animation_new(void)
{
	Etch_Animation *a;
	
	a = calloc(1, sizeof(Etch_Animation));
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
EAPI void etch_animation_mark_add(Etch_Animation *a, Etch_Animation_Keyframe *m)
{
	/* check the mark's time and place it like start or end if necessary */
}

/**
 * Delete the mark from the animation
 */
EAPI void etch_animation_mark_del(Etch_Animation *a, Etch_Animation_Keyframe *m)
{
	/* remove the mark from the list and recalculate the start and end if necessary */
}

/* TODO do we need to be this finegrained? */

/**
 * Set the time for a mark
 */
EAPI void etch_animation_mark_time_set(Etch_Animation_Keyframe *m)
{
	
}

/**
 * Set the value for a mark
 */
EAPI void etch_animation_mark_value_set(Etch_Animation_Keyframe *m, int type)
{
	
}
