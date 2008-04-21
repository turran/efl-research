#include "Etch.h"
#include "etch_private.h"

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
	Eina_Inlist *l;
	int i;
	
	/* iterate over the list of objects to get the animations */
	for (l = (Eina_Inlist *)e->objects; l; l = l->next)
	{
		Etch_Object *o = (Etch_Object *)l;
		
		/* iterate over the list of animations */
		for (i = 0; i < o->nprops; i++)
		{
			Etch_Animation *a;
			
			
			if (!(o->animations[i]))
				continue;
			/* check that the animation start and end is between our time */
			a = o->animations[i];
			if ((e->curr >= a->start) && (e->curr <= a->end))
			{
				void *pdata;
				Etch_Property_Set setfnc;

				//printf("curr = %g, start = %g, end = %g\n", e->curr, a->start, a->end);
				/* get the property offset */
				pdata = ((char *)o->props
						+ o->offsets[i]);
				etch_animation_data_animate(a, pdata, e->curr);
				/* once the value has been set, call the callback */
				setfnc = o->oclass->props[i].set;
				setfnc(o->data, pdata);
			}
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
	e->objects = NULL;
	e->frame = 0;
	e->curr = 0;
	return e;
}

/**
 * Delete the Etch instance
 */
EAPI void etch_free(Etch *e)
{
	assert(e);
	/* remove every object */
	free(e);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_timer_fps_set(Etch *e, unsigned int fps)
{
	assert(e);
	e->fps = fps;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI unsigned int etch_timer_fps_get(Etch *e)
{
	assert(e);
	return e->fps;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_timer_tick(Etch *e)
{
	assert(e);
	/* TODO check for overflow */
	e->frame++;
	e->curr += (double)1/e->fps;
	_process(e);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int etch_timer_has_end(Etch *e)
{
	/* we need a function to get the last frame/marker to know when the
	 * animations have finished, on the application we can make it run again,
	 * stop, whatever */
	
	return 0;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_timer_goto(Etch *e, unsigned long frame)
{
	e->frame = frame;
	e->curr = (double)frame/e->fps;
	_process(e);
}
