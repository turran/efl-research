#include "Etch.h"
#include "etch_private.h"

/* REMOVE THIS */
extern Etch_Object *_object;

/**
 * TODO
 * + maybe a function to call whenever the fps timer has match? like:
 * etc_timer_notify(Etch *)
 * + maybe remove the _timer_ prefix?
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _fps_to_time(unsigned long frame, unsigned long *time)
{
	/* giving a frame transform it to secs|usec representation */ 
}

static void _process(Etch *e)
{
	int i;
	/* iterate over the list of objects to get the animations */
	/* TODO for now get the referenced object */
	/* iterate over the list of animations */
	for (i = 0; i < _object->nprops; i++)
	{
		Etch_Animation *a;
		
		if (!(_object->animations[i])) continue;
		/* check that the animation start and end is between our time */
		a = _object->animations[i];
		if ((e->curr >= a->start) && (e->curr <= a->end))
		{
			void *pdata;
			Etch_Property_Set setfnc;
			
			//printf("curr = %g, start = %g, end = %g\n", e->curr, a->start, a->end);
			/* get the property offset */
			pdata = ((char *)_object->props + _object->offsets[i]);
			etch_animation_data_animate(a, pdata, e->curr);
			/* once the value has been set, call the callback */
			setfnc = _object->oclass->props[i].set;
			setfnc(_object->data, pdata);
		}
	}	
}
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
 * Delete the Etch instance
 */
EAPI void etch_free(Etch *e)
{
	/* remove every object */
	free(e);
}


/**
 * 
 */
EAPI void etch_timer_fps_set(Etch *e, unsigned int fps)
{
	e->fps = fps;
}

/**
 * 
 */
EAPI unsigned int etch_timer_fps_get(Etch *e)
{
	return e->fps;
}

/**
 * 
 */
EAPI void etch_timer_tick(Etch *e)
{
	/* TODO check for overflow */
	e->frame++;
	e->curr += (double)1/e->fps;
	_process(e);
}

/**
 * 
 */
EAPI int etch_timer_has_end(Etch *e)
{
	/* we need a function to get the last frame/marker to know when the
	 * animations have finished, on the application we can make it run again,
	 * stop, whatever */
	return 0;
}

/**
 * 
 */
EAPI void etch_timer_goto(Etch *e, unsigned long frame)
{
	e->frame = frame;
	e->curr = (double)frame/e->fps;
	_process(e);
}
