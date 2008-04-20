#ifndef ENESIM_TRANSFORMER_H_
#define ENESIM_TRANSFORMER_H_

/**
 * @todo
 * + Add a function to compose the transformations
 * + Add a function to setup the quality
 */
typedef struct _Enesim_Transformation Enesim_Transformation;

EAPI Enesim_Transformation * enesim_transformation_new(void);
EAPI void enesim_transformation_set(Enesim_Transformation *t, float *tx);
EAPI void enesim_transformation_apply(Enesim_Transformation *t, Enesim_Surface *s, Enesim_Rectangle *sr, Enesim_Surface *d, Enesim_Rectangle *dr);

#endif /*ENESIM_TRANSFORMER_H_*/
