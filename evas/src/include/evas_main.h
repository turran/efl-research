#ifndef EVAS_MAIN_H_
#define EVAS_MAIN_H_

/**
 * @defgroup Evas_Core_Group Core
 * @{
 */
typedef int Evas_Coord; /**< */
typedef char Evas_Bool; /**< */
typedef struct _Evas Evas; /**< An Evas canvas handle */

EAPI int               evas_init                         (void);
EAPI int               evas_shutdown                     (void);
EAPI Evas             *evas_new                          (void);
EAPI void              evas_free                         (Evas *e);
EAPI void              evas_output_size_set              (Evas *e, int w, int h);
EAPI void              evas_output_size_get              (const Evas *e, int *w, int *h);
EAPI void              evas_output_viewport_set          (Evas *e, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h);
EAPI void              evas_output_viewport_get          (const Evas *e, Evas_Coord *x, Evas_Coord *y, Evas_Coord *w, Evas_Coord *h);
/*
 * EAPI void              evas_output_method_set            (Evas *e, int render_method);
 * EAPI int               evas_output_method_get            (const Evas *e);
 */

/** 
 * @}
 */

#endif /*EVAS_MAIN_H_*/
