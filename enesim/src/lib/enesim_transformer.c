#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

#include "fixed_16p16.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/


static Enesim_Transformer_Type _transformation_get(float *t)
{
	if ((t[MATRIX_ZX] != 0) || (t[MATRIX_ZY] != 0) || (t[MATRIX_ZZ] != 1))
	            return ENESIM_TRANSFORMATION_PROJECTIVE;
	else
	{
		if ((t[MATRIX_XX] == 1) && (t[MATRIX_XY] == 0) && (t[MATRIX_XZ] == 0) &&
				(t[MATRIX_YX] == 0) && (t[MATRIX_YY] == 1) && (t[MATRIX_YZ] == 0))
			return ENESIM_TRANSFORMATION_IDENTITY;
		else
			return ENESIM_TRANSFORMATION_AFFINE;
	}
}

/* convert the transformation values to fixed point */
static void _transformation_to_fixed(float *t, enesim_16p16_t *td)
{
	td[0] = enesim_16p16_float_from(t[0]);
	td[1] = enesim_16p16_float_from(t[1]);
	td[2] = enesim_16p16_float_from(t[2]);
	td[3] = enesim_16p16_float_from(t[3]);
	td[4] = enesim_16p16_float_from(t[4]);
	td[5] = enesim_16p16_float_from(t[5]);
	td[6] = enesim_16p16_float_from(t[6]);
	td[7] = enesim_16p16_float_from(t[7]);
	td[8] = enesim_16p16_float_from(t[8]);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI Enesim_Transformation * enesim_transformation_new(void)
{
	Enesim_Transformation *t;
	
	t = calloc(1, sizeof(Enesim_Transformation));
	/* setup the identity matrix */
	t->matrix[MATRIX_XX] = 1;
	t->matrix[MATRIX_YY] = 1;
	t->matrix[MATRIX_ZZ] = 1;
	t->type = ENESIM_TRANSFORMATION_IDENTITY;
	
	return t;
}
/**
 * 
 */
EAPI void enesim_transformation_set(Enesim_Transformation *t, float *tx)
{
	int i;
		
	assert(t);
	
	/* TODO set the type of the transformation */
	for (i = 0; i < MATRIX_SIZE; i++)
	{
		t->matrix[i] = tx[i];
	}
}
/**
 * 
 */
EAPI void enesim_transformation_apply(Enesim_Transformation *t, Enesim_Surface *s, Enesim_Rectangle *sr, Enesim_Surface *d, Enesim_Rectangle *dr)
{
	Enesim_Rectangle csr, cdr;
	enesim_16p16_t ft[MATRIX_SIZE];
		
	assert(s);
	assert(d);

	/* setup the destination clipping */
	cdr.x = 0;
	cdr.y = 0;
	cdr.w = d->w;
	cdr.h = d->h;
	if (sr)
	{
		/* TODO check the return value of the intersection */
		enesim_rectangle_rectangle_intersection_get(&cdr, dr);
		if (enesim_rectangle_is_empty(&cdr))
			return;
	}
	/* setup the source clipping */
	csr.x = 0;
	csr.y = 0;
	csr.w = s->w;
	csr.h = s->h;
	if (dr)
	{
		/* TODO check the return value of the intersection */
		enesim_rectangle_rectangle_intersection_get(&csr, sr);
		if (enesim_rectangle_is_empty(&csr))
			return;
	}
	/* if we need to do some calcs with the matrix, transform it */
	// if (!(s->transformation.type & ENESIM_SURFACE_TRANSFORMATION_IDENTITY))
	_transformation_to_fixed(t->matrix, ft);
	
	/* check if we are going to scale */
	/* scaling */
	if ((cdr.w != csr.w) && (cdr.h != csr.h))
	{
		/* smooth scaling */
		if ((cdr.w > csr.w) && (cdr.h <= csr.h))
		{
			/* x upscaling, y downscaling */
		}
		else if ((cdr.w <= csr.w) && (cdr.h > csr.h))
		{
			/* x downscaling, y upscaling */
		}
		else if ((cdr.w > csr.w) && (cdr.h > csr.h))
		{
			/* x upscaling, y upscaling */
		}
	}
	/* not scaling */
	else
	{
		/* when using the multiplied variant check that the mul color is different
		 * than 255,255,255,255, if not use the version without mul
		 */
		//_backends[s->format].draw[s->transformation.type][ENESIM_SURFACE_NO_SCALE];
		/* check transformation type */
		//argb8888_transformed_blend(s, &csr, d, &cdr, ft);
		//argb8888_identity_op(s, d);
		//argb8888_c_draw_fill_affine_no_no(s, &csr, d, &cdr, ft);
		//argb8888_c_draw_blend_mul_affine_no_no(s, &csr, d, &cdr, ft, 0xcccccccc);
	}
	/* get the correct drawer function */
	/* get the correct transfomer function */
}
