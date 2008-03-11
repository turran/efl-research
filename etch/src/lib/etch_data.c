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
	/* return(a*(1-m)+b*m); where m = [0,1] */
}

static void _cosin_uint32(void *data, double m, unsigned int a, unsigned int b, void *res)
{
	/* double m2;
	 * m2 = (1-cos(m*PI))/2;
	 * return(a*(1-m2)+b*m2);
	 * where m = [0, 1]
	 */
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * 
 */
void etch_data_interpolate(unsigned long time, Etch_Animation_Keyframe *start, Etch_Animation_Keyframe *end)
{
	double m;
	
	/* TODO Fix how time will get passed */
	/* get the interval between 0 and 1 based on current frame and two keyframes */
	//m = (time - start->time)/(end->time - start->time);
	/* get the data offset */
	/* call the desired function based on the start keyframe interpolation type */
}
