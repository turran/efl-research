/*
 * eon_image.h
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */
#ifndef EON_IMAGE_H_
#define EON_IMAGE_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_IMAGE_FILE_CHANGED "fileChanged"
#define EON_IMAGE_MATRIX_CHANGED "matrixChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_IMAGE_FILE;
extern Ekeko_Property_Id EON_IMAGE_MATRIX;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Image_Private Eon_Image_Private;
struct _Eon_Image
{
	Eon_Square parent;
	Eon_Image_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_image_type_get(void);
EAPI Eon_Image * eon_image_new(Eon_Canvas *c);

EAPI void eon_image_file_set(Eon_Image *i, const char *file);
EAPI const char * eon_image_file_get(Eon_Image *i);
EAPI void eon_image_matrix_set(Eon_Image *i, Enesim_Matrix *m);
/* square wrappers */
#define eon_image_x_rel_set(i, x) eon_square_x_rel_set((Eon_Square *)(i), x)
#define eon_image_y_rel_set(i, y) eon_square_y_rel_set((Eon_Square *)(i), y)
#define eon_image_w_rel_set(i, w) eon_square_w_rel_set((Eon_Square *)(i), w)
#define eon_image_h_rel_set(i, h) eon_square_h_rel_set((Eon_Square *)(i), h)
#define eon_image_x_set(i, x) eon_square_x_set((Eon_Square *)(i), x)
#define eon_image_y_set(i, y) eon_square_y_set((Eon_Square *)(i), y)
#define eon_image_w_set(i, w) eon_square_w_set((Eon_Square *)(i), w)
#define eon_image_h_set(i, h) eon_square_h_set((Eon_Square *)(i), h)
/* shape wrappers */
#define eon_image_color_set(i, c) eon_shape_color_set((Eon_Shape *)(i), c)
#define eon_image_rop_set(i, r) eon_shape_rop_set((Eon_Shape *)(i), r)
/* renderable wrappers */
#define eon_image_show(i) ekeko_renderable_show(EKEKO_RENDERABLE((i)))
#define eon_image_hide(i) ekeko_renderable_hide(EKEKO_RENDERABLE((i)))

#endif /* EON_IMAGE_H_ */
