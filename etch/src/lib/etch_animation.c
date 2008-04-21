#include "Etch.h"
#include "etch_private.h"

/**
 * @todo
 * - make functions to interpolate between data types,
 * possible animations:
 * sin
 * exp
 * log
 * linear
 * bezier based (1 and 2 control points)
 * - make every interpolator work for every data type, so better a function table
 * - define animatinos based on two properties: PERIODIC, UNIQUE, PERIODIC_MIRROR
 * - the integer return values of the interpolators should be rounded?
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _keyframe_debug(Etch_Animation_Keyframe *k)
{
	printf("Keyframe at %g of type %d\n", k->time, k->type);
	switch (k->animation->dtype)
	{
		case ETCH_UINT32:
		printf("value = %u\n", k->value.u32);
		break;

		case ETCH_ARGB:
		printf("value = 0x%8x\n", k->value.argb);
		break;
		
		default:
		break;
	}
}

static void _animation_debug(Etch_Animation *a)
{
	Eina_Inlist *l;

	printf("Animation that interpolates data of type %d, with the following keyframes:\n", a->dtype);
	l = (Eina_Inlist *)a->keys;
	while (l)
	{
		Etch_Animation_Keyframe *k = (Etch_Animation_Keyframe *)l;
	
		_keyframe_debug(k);
		l = l->next;
	}
}

static void _linear_argb(void *data, double m, Etch_Data *da, Etch_Data *db, unsigned int *res)
{
	unsigned int range;
	unsigned int a, b, ag, rb;
	
	a = da->argb;
	b = db->argb;
	
	/* handle specific case where a and b are equal (constant) */
	if (a == b)
	{
		*res = a;
		return;
	}
	/* b - a*m + a */
	range = rint(256 * m);
	ag = ((((((b >> 8) & 0xff00ff) - ((a >> 8) & 0xff00ff)) * range) + (a & 0xff00ff00)) & 0xff00ff00);  
	rb = ((((((b & 0xff00ff) - (a & 0xff00ff)) * (range)) >> 8) + (a & 0xff00ff)) & 0xff00ff);
	
	*res = ag + rb;
}

static void _linear_uint32(void *data, double m, Etch_Data *da, Etch_Data *db, unsigned int *res)
{
	double r;
	unsigned int a, b;
	
	a = da->u32;
	b = db->u32;
	
	/* handle specific case where a and b are equal (constant) */
	if (a == b)
	{
		*res = a;
		return;
	}
	r = ((1 - m) * a) + (m * b);
	*res = ceil(r);
}

static void _cosin_uint32(void *data, double m, Etch_Data *da, Etch_Data *db, unsigned int *res)
{
	double m2;
	unsigned int a, b;
		
	a = da->u32;
	b = db->u32;
	
	m2 = (1 - cos(m * M_PI))/2;
	
	*res = ceil((double)(a * (1 - m2) + b * m2));
}

static void _bquad_uint32(void *data, double m, Etch_Data *da, Etch_Data *db, unsigned int *res)
{
	Etch_Animation_Quadratic *q = data;
	unsigned int a, b;
		
	a = da->u32;
	b = db->u32;
	
	*res =  (1 - m) * (1 - m) * a + 2 * m * (1 - m) * (q->cp.u32) + m * m * b;
}

static void _bcubic_uint32(void *data, double m, Etch_Data *da, Etch_Data *db, unsigned int *res)
{
	/* 
	 */
}
/* prototype of the function table */
typedef void (*Etch_Interpolator)(void *data, double m, Etch_Data *a, Etch_Data *b, void *res);

Etch_Interpolator _interpolators[ETCH_ANIMATION_TYPES][ETCH_DATATYPES] = {
		[ETCH_ANIMATION_LINEAR][ETCH_UINT32] = (Etch_Interpolator)_linear_uint32,
		[ETCH_ANIMATION_COSIN][ETCH_UINT32] = (Etch_Interpolator)_cosin_uint32,
		[ETCH_ANIMATION_QUADRATIC][ETCH_UINT32] = (Etch_Interpolator)_bquad_uint32,
		[ETCH_ANIMATION_LINEAR][ETCH_ARGB] = (Etch_Interpolator)_linear_argb,
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
void etch_animation_data_animate(Etch_Animation *a, void *pdata, double curr)
{
	Eina_Inlist *l;
	Etch_Animation_Keyframe *start;
	Etch_Animation_Keyframe *end;
	
	/* check that the time is between two keyframes */
	if (!a->keys)
		return;
	
	/* TODO instead of checking everytime every keyframe we can translate the
	 * keyframes based on the frame, when a keyframe has passed move it before
	 * like a circular list */
	l = (Eina_Inlist *)a->keys;
	while (l)
	{
		start = (Etch_Animation_Keyframe *)l;
		end = (Etch_Animation_Keyframe *)(l->next);
		if (!end)
			break;
		if ((curr >= start->time) && (curr <= end->time))
		{
			Etch_Interpolator ifnc;
			double m;
					
			/* get the interval between 0 and 1 based on current frame and two keyframes */
			m = (curr - start->time)/(end->time - start->time);
			/* accelerate the calculations if we get the same m as the previous call */
			if (m == a->m)
			{
				/* TODO instead of return call the set callback again? */
				return;
			}
			/* interpolate the new value */
			ifnc = _interpolators[start->type][a->dtype];
			ifnc(&start->data, m, &(start->value), &(end->value), pdata);
		}
		l = l->next;
	}
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * Create a new animation
 * @param dtype Data type the animation will animate
 */
EAPI Etch_Animation * etch_animation_new(Etch_Data_Type dtype)
{
	Etch_Animation *a;
	
	a = calloc(1, sizeof(Etch_Animation));
	/* common values */
	a->m = -1; /* impossible, so the first keyframe will overwrite this */
	a->start = DBL_MAX;
	a->dtype = dtype;
	
	return a;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etch_animation_free(Etch_Animation *a)
{
	assert(a);
	/* TODO delete the list of keyframes */
	free(a);
}
/**
 * Add a new mark to the animation
 */
EAPI Etch_Animation_Keyframe * etch_animation_keyframe_add(Etch_Animation *a)
{
	Etch_Animation_Keyframe *k;
	
	assert(a);
	k = calloc(1, sizeof(Etch_Animation_Keyframe));
	k->animation = a;

	/* add the new keyframe to the list of keyframes */
	a->keys = eina_inlist_prepend(a->keys, k);
	
	return k;
}
/**
 * Delete the mark from the animation
 */
EAPI void etch_animation_keyframe_del(Etch_Animation *a, Etch_Animation_Keyframe *k)
{
	assert(a);
	assert(k);
	/* remove the keyframe from the list */
	a->keys = eina_inlist_remove(a->keys, k);
	/* TODO recalculate the start and end if necessary */
	free(k);
}
/**
 * Set the type of animation keyframe
 */
EAPI void etch_animation_keyframe_type_set(Etch_Animation_Keyframe *k, Etch_Animation_Type t)
{
	assert(k);
	k->type = t;
}
/**
 * Get the type of animation keyframe
 */
EAPI Etch_Animation_Type etch_animation_keyframe_type_get(Etch_Animation_Keyframe *k)
{
	assert(k);
	return k->type;
}
/**
 * Set the time for a mark
 */
EAPI void etch_animation_keyframe_time_set(Etch_Animation_Keyframe *k, unsigned long secs, unsigned long usecs)
{
	Etch_Animation *a;
	struct timeval t;
	double new_time;
	Eina_Inlist *l;
	
	assert(k);
	
	t.tv_sec = secs;
	t.tv_usec = usecs;
	new_time = etch_timeval_to_double(&t);
	/* if the time is the same, do nothing */
	if (new_time == k->time)
		return;
	a = k->animation;
	/* find the greater element with the value less than the one to set */
	l = (Eina_Inlist *)(a->keys);
	while (l)
	{
		Etch_Animation_Keyframe *k2 = (Etch_Animation_Keyframe *)l;
		
		if (k2->time >= new_time)
			break;
		l = l->next;
	}
	/* if the element to remove is the same as the element to change, do
	 * nothing */
	if ((Etch_Animation_Keyframe*)l == k)
		goto update;
	a->keys = eina_inlist_remove(a->keys, k);
	/* k is the greatest */
	if (!l)
	{
		a->keys = eina_inlist_append(a->keys, k);
	}
	/* k is between two keyframes */
	else
	{	
		a->keys = eina_inlist_prepend_relative(a->keys, k, l);	
	}
	/* update the start and end values */
update:
	k->time = new_time;
	a->start = a->keys->time;
	a->end = ((Etch_Animation_Keyframe *)((Eina_Inlist *)(a->keys))->last)->time;
}
/**
 * Get the value for a mark
 */ 
EAPI void etch_animation_keyframe_value_get(Etch_Animation_Keyframe *k, ...)
{
	assert(k);
	/* TODO */
}
/**
 * Set the value for a mark
 */
EAPI void etch_animation_keyframe_value_set(Etch_Animation_Keyframe *k, ...)
{
	va_list va;

	assert(k);
	va_start(va, k);
	/* now get the type specific data, for example the bezier forms need 
	 * control points, etc */
	switch (k->type)
	{	
		case ETCH_ANIMATION_LINEAR:
		{
			switch (k->animation->dtype)
			{	
				case ETCH_UINT32:
					k->value.u32 = va_arg(va, unsigned int);
					break;
				case ETCH_ARGB:
					k->value.argb = va_arg(va, unsigned int);
					break;
				default:
					break;
			}
			break;
		}
		case ETCH_ANIMATION_QUADRATIC:
		{
			switch (k->animation->dtype)
			{	
				case ETCH_UINT32:
					k->value.u32 = va_arg(va, unsigned int);
					k->data.q.cp.u32 = va_arg(va, unsigned int);
					break;
				default:
					break;
			}
			break;
		}
		case ETCH_ANIMATION_CUBIC:
		{
			switch (k->animation->dtype)
			{	
				case ETCH_UINT32:
					k->value.u32 = va_arg(va, unsigned int);
					k->data.c.cp1.u32 = va_arg(va, unsigned int);
					k->data.c.cp2.u32 = va_arg(va, unsigned int);
					break;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
	va_end(va);		
}
