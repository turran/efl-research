#include "Etch.h"
#include "etch_private.h"

/* 
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
/* prototype of the function table */
typedef void (*Etch_Interpolator)(void *data, double m, void *res);

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
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * 
 */
void etch_data_interpolate(Etch_Object *eo, Etch_Animation_Keyframe *start, Etch_Animation_Keyframe *end, double t)
{
	Etch_Animation *a;
	double m;
	
	/* get the interval between 0 and 1 based on current frame and two keyframes */
	m = (t - start->time)/(end->time - start->time);
	/* accelerate the calculations if we get the same m as the previous call */
	if (m == start->animation->m)
		return;
	
	a = start->animation;
	/* get the data offset */
	/* call the desired function based on the start keyframe interpolation type */
}
