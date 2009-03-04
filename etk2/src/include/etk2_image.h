/*
 * etk2_image.h
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */
#ifndef ETK2_IMAGE_H_
#define ETK2_IMAGE_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_IMAGE_FILE_CHANGED "fileChanged"
#define ETK_IMAGE_MATRIX_CHANGED "matrixChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_IMAGE_FILE;
extern Property_Id ETK_IMAGE_MATRIX;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Image_Private Etk_Image_Private;
struct _Etk_Image
{
	Etk_Square parent;
	Etk_Image_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_image_type_get(void);
EAPI Etk_Image * etk_image_new(Etk_Canvas *c);

EAPI void etk_image_file_set(Etk_Image *i, const char *file);
EAPI const char * etk_image_file_get(Etk_Image *i);
EAPI void etk_image_matrix_set(Etk_Image *i, Enesim_Matrix *m);
#define etk_image_x_rel_set(i, x) etk_square_x_rel_set((Etk_Square *)(i), x)
#define etk_image_y_rel_set(i, y) etk_square_y_rel_set((Etk_Square *)(i), y)
#define etk_image_w_rel_set(i, w) etk_square_w_rel_set((Etk_Square *)(i), w)
#define etk_image_h_rel_set(i, h) etk_square_h_rel_set((Etk_Square *)(i), h)
#define etk_image_x_set(i, x) etk_square_x_set((Etk_Square *)(i), x)
#define etk_image_y_set(i, y) etk_square_y_set((Etk_Square *)(i), y)
#define etk_image_w_set(i, w) etk_square_w_set((Etk_Square *)(i), w)
#define etk_image_h_set(i, h) etk_square_h_set((Etk_Square *)(i), h)

#endif /* ETK2_IMAGE_H_ */
