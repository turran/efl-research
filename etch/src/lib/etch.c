#include "Etch.h"
#include "etch_private.h"

/**
 * TODO
 * + maybe a function to call whenever the fps timer has match? like:
 * etc_timer_notify(Etch *)
 */
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * Create a new Etch instance
 */
EAPI Etch * etch_new(void)
{
	Etch *e;
	
	e = malloc(sizeof(Etch));
	return e;
}

/**
 * 
 */
EAPI void etch_free(Etch *e)
{
	free(e);
}

/**
 * 
 */
EAPI void etch_timer_fps_set(Etch *e, int fps)
{
	
}

/**
 * 
 */
EAPI int etch_timer_fps_get(Etch *e)
{
	
}

/**
 * 
 */
EAPI void etch_timer_tick(Etch *e)
{
	/* increment the frame */
	/* iterate over the list of objects to get the animations */
	/* iterate over the list of animations */
	/* check that the animation is between the two markers */
	/* if it is, calculate the appropiate property and call the property set function */
}

/**
 * 
 */
EAPI int etch_timer_has_end(Etch *e)
{
	/* we need a function to get the last frame/marker to know when the
	 * animations have finished, on the application we can make it run again,
	 * stop, whatever */
}

/**
 * 
 */
EAPI void etch_timer_goto(Etch *e, unsigned long frame)
{
	
}
