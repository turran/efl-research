/*
 * eon_hswitch.h
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */
#ifndef EON_HSWITCH_H_
#define EON_HSWITCH_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_HSWITCH_FILE_CHANGED "fileChanged"
#define EON_HSWITCH_LOADED_CHANGED "loadedChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_HSWITCH_FILE;
extern Ekeko_Property_Id EON_HSWITCH_LOADED;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Hswitch_Private Eon_Hswitch_Private;
struct _Eon_Hswitch
{
	Eon_Paint parent;
	Eon_Hswitch_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *eon_hswitch_type_get(void);
EAPI Eon_Hswitch * eon_hswitch_new(Eon_Canvas *c);

EAPI void eon_hswitch_left_set(Eon_Hswitch *i, const char *file);
EAPI void eon_hswitch_right_set(Eon_Hswitch *i, const char *file);

/* paint wrappers */
#define eon_hswitch_matrix_set(i, m) eon_paint_matrix_set((Eon_Paint *)(i), m)
#define eon_hswitch_x_rel_set(i, x) eon_paint_x_rel_set((Eon_Paint *)(i), x)
#define eon_hswitch_y_rel_set(i, y) eon_paint_y_rel_set((Eon_Paint *)(i), y)
#define eon_hswitch_w_rel_set(i, w) eon_paint_w_rel_set((Eon_Paint *)(i), w)
#define eon_hswitch_h_rel_set(i, h) eon_paint_h_rel_set((Eon_Paint *)(i), h)
#define eon_hswitch_x_set(i, x) eon_paint_x_set((Eon_Paint *)(i), x)
#define eon_hswitch_y_set(i, y) eon_paint_y_set((Eon_Paint *)(i), y)
#define eon_hswitch_w_set(i, w) eon_paint_w_set((Eon_Paint *)(i), w)
#define eon_hswitch_h_set(i, h) eon_paint_h_set((Eon_Paint *)(i), h)

#endif /* EON_HSWITCH_H_ */
