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
EAPI void enesim_transformation_rop_set(Enesim_Transformation *t, Enesim_Rop rop);
EAPI Eina_Bool enesim_transformation_apply(Enesim_Transformation *t, Enesim_Surface *s, Eina_Rectangle *sr, Enesim_Surface *d, Eina_Rectangle *dr);
EAPI void enesim_transformation_matrix_compose(float *st, float *dt);
EAPI void enesim_transformation_matrix_translate(float *t, float tx, float ty);
EAPI void enesim_transformation_matrix_scale(float *t, float sx, float sy);
EAPI void enesim_transformation_matrix_rotate(float *t, float rad);
EAPI void enesim_transformation_matrix_identity(float *t);




#endif /*ENESIM_TRANSFORMER_H_*/
