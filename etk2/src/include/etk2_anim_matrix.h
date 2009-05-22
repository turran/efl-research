/*
 * etk2_anim_matrix.h
 *
 *  Created on: 22-may-2009
 *      Author: jl
 */

#ifndef ETK2_ANIM_MATRIX_H_
#define ETK2_ANIM_MATRIX_H_
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_ANIMATION_MATRIX_TYPE;
typedef enum Etk_Animation_Matrix_Type
{
	ETK_ROTATE,
	ETK_SCALE,
	ETK_ANIMATION_MATRIX_TYPES,
} Etk_Animation_Matrix_Type;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Animation_Matrix_Private Etk_Animation_Matrix_Private;
struct _Etk_Animation_Matrix
{
	Etk_Animation parent;
	Etk_Animation_Matrix_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type * etk_animation_matrix_type_get(void);
EAPI Etk_Animation_Matrix * etk_animation_matrix_new(void);

#endif /* ETK2_ANIM_MATRIX_H_ */
