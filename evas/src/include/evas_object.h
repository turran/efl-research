#ifndef EVAS_OBJECT_H_
#define EVAS_OBJECT_H_

/**
 * @defgroup Evas_Object_Group Objects
 * @{
 */
typedef enum _Evas_Render_Op
{
	EVAS_RENDER_BLEND = 0, /**< default op: d = d*(1-sa) + s */
	EVAS_RENDER_BLEND_REL = 1, /**< d = d*(1 - sa) + s*da */
	EVAS_RENDER_COPY = 2, /**< d = s */
	EVAS_RENDER_COPY_REL = 3, /**< d = s*da */
	EVAS_RENDER_ADD = 4, /**< d = d + s */
	EVAS_RENDER_ADD_REL = 5, /**< d = d + s*da */
	EVAS_RENDER_SUB = 6, /**< d = d - s */
	EVAS_RENDER_SUB_REL = 7, /**< d = d - s*da */
	EVAS_RENDER_TINT = 8, /**< d = d*s + d*(1 - sa) + s*(1 - da) */
	EVAS_RENDER_TINT_REL = 9, /**< d = d*(1 - sa + s) */
	EVAS_RENDER_MASK = 10, /**< d = d*sa */
	EVAS_RENDER_MUL = 11 /**< d = d*s */
} Evas_Render_Op; /**<  */

typedef struct _Evas_Object Evas_Object; /**< An Evas Object handle */
typedef struct _Evas_List Evas_List;

EAPI void              evas_object_del                   (Evas_Object *obj);

EAPI const char       *evas_object_type_get              (const Evas_Object *obj);

EAPI void              evas_object_layer_set             (Evas_Object *obj, int l);
EAPI int               evas_object_layer_get             (const Evas_Object *obj);

EAPI void              evas_object_raise                 (Evas_Object *obj);
EAPI void              evas_object_lower                 (Evas_Object *obj);
EAPI void              evas_object_stack_above           (Evas_Object *obj, Evas_Object *above);
EAPI void              evas_object_stack_below           (Evas_Object *obj, Evas_Object *below);
EAPI Evas_Object      *evas_object_above_get             (const Evas_Object *obj);
EAPI Evas_Object      *evas_object_below_get             (const Evas_Object *obj);
EAPI Evas_Object      *evas_object_bottom_get            (const Evas *e);
EAPI Evas_Object      *evas_object_top_get               (const Evas *e);

EAPI void              evas_object_move                  (Evas_Object *obj, Evas_Coord x, Evas_Coord y);
EAPI void              evas_object_resize                (Evas_Object *obj, Evas_Coord w, Evas_Coord h);
EAPI void              evas_object_geometry_get          (const Evas_Object *obj, Evas_Coord *x, Evas_Coord *y, Evas_Coord *w, Evas_Coord *h);

EAPI void              evas_object_show                  (Evas_Object *obj);
EAPI void              evas_object_hide                  (Evas_Object *obj);
EAPI Evas_Bool         evas_object_visible_get           (const Evas_Object *obj);

EAPI void              evas_object_render_op_set         (Evas_Object *obj, Evas_Render_Op op);
EAPI Evas_Render_Op    evas_object_render_op_get         (const Evas_Object *obj);

EAPI void              evas_object_anti_alias_set        (Evas_Object *obj, Evas_Bool antialias);
EAPI Evas_Bool         evas_object_anti_alias_get        (const Evas_Object *obj);

EAPI void              evas_object_color_set             (Evas_Object *obj, int r, int g, int b, int a);
EAPI void              evas_object_color_get             (const Evas_Object *obj, int *r, int *g, int *b, int *a);

EAPI void              evas_object_color_interpolation_set  (Evas_Object *obj, int color_space);
EAPI int               evas_object_color_interpolation_get  (const Evas_Object *obj);

EAPI void              evas_object_clip_set              (Evas_Object *obj, Evas_Object *clip);
EAPI Evas_Object      *evas_object_clip_get              (const Evas_Object *obj);
EAPI void              evas_object_clip_unset            (Evas_Object *obj);
EAPI const Evas_List  *evas_object_clipees_get           (const Evas_Object *obj);

EAPI void              evas_object_data_set              (Evas_Object *obj, const char *key, const void *data);
EAPI void             *evas_object_data_get              (const Evas_Object *obj, const char *key);
EAPI void             *evas_object_data_del              (Evas_Object *obj, const char *key);

EAPI void              evas_object_name_set              (Evas_Object *obj, const char *name);
EAPI const char       *evas_object_name_get              (const Evas_Object *obj);
EAPI Evas_Object      *evas_object_name_find             (const Evas *e, const char *name);

EAPI Evas             *evas_object_evas_get              (const Evas_Object *obj);

EAPI Evas_Object      *evas_object_top_at_xy_get         (const Evas *e, Evas_Coord x, Evas_Coord y, Evas_Bool include_pass_events_objects, Evas_Bool include_hidden_objects);
EAPI Evas_Object      *evas_object_top_at_pointer_get    (const Evas *e);
EAPI Evas_Object      *evas_object_top_in_rectangle_get  (const Evas *e, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, Evas_Bool include_pass_events_objects, Evas_Bool include_hidden_objects);

EAPI Evas_List        *evas_objects_at_xy_get            (const Evas *e, Evas_Coord x, Evas_Coord y, Evas_Bool include_pass_events_objects, Evas_Bool include_hidden_objects);
EAPI Evas_List        *evas_objects_in_rectangle_get     (const Evas *e, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, Evas_Bool include_pass_events_objects, Evas_Bool include_hidden_objects);

/** 
 * @}
 */

#ifdef EVAS_MODULE

struct _Evas_Object_Func
{
	void (*free) (Evas_Object *obj);
	void (*render) (Evas_Object *obj, void *output, void *context, void *surface, int x, int y);
	void (*render_pre) (Evas_Object *obj);
	void (*render_post) (Evas_Object *obj);

	void (*store) (Evas_Object *obj);
	void (*unstore) (Evas_Object *obj);

	int (*is_visible) (Evas_Object *obj);
	int (*was_visible) (Evas_Object *obj);

	int (*is_opaque) (Evas_Object *obj);
	int (*was_opaque) (Evas_Object *obj);

	int (*is_inside) (Evas_Object *obj, Evas_Coord x, Evas_Coord y);
	int (*was_inside) (Evas_Object *obj, Evas_Coord x, Evas_Coord y);

	void (*coords_recalc) (Evas_Object *obj);
};

#endif

#endif /*EVAS_OBJECT_H_*/
