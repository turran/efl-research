#ifndef EVAS_PRIVATE_H
#define EVAS_PRIVATE_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "Evas.h"

/* the evas module api */
/***********************/
/* the module api version */
#define EVAS_MODULE_API_VERSION 1

/* the module types */
typedef enum _Evas_Module_Type
{
   EVAS_MODULE_TYPE_ENGINE,
   EVAS_MODULE_TYPE_IMAGE_LOADER,
   EVAS_MODULE_TYPE_IMAGE_SAVER,
     EVAS_MODULE_TYPE_OBJECT
} Evas_Module_Type;

/* the module api structure, all modules should define this struct */
typedef struct _Evas_Module_Api Evas_Module_Api;
struct _Evas_Module_Api
{
   int			version;
   Evas_Module_Type	type;
   const char 		*name;
   const char		*author;
};

/* the module structure */
typedef struct _Evas_Module Evas_Module;
struct _Evas_Module
{
   Evas_Module_Api 	*api;	
   void			*handle;	/* the dlopen handle */
   char			*path;		/* the path where this modules is */
   char			*name;		/* the name of the dir where this module is */
   struct
     {
	int (*open)(Evas_Module *);
	void (*close)(Evas_Module *);
     } func;
   void		*functions;	/* this are the functions exported by the module */
   void		*data;		/* some internal data for the module i.e the id for engines */

   Evas_Module_Type	type;		/* the type detected by the path */
   
   int           ref; /* how many refs */
   int           last_used; /* the cycle count when it was last used */
   
   unsigned char	loaded : 1;
};


/* the internals of the module api use this struct to reference a path with a module type
 * instead of deduce the type from the path. 
 * */
typedef struct _Evas_Module_Path Evas_Module_Path;
struct _Evas_Module_Path
{
   Evas_Module_Type	type;
   char		       *path;
};

typedef struct _Evas_Module_Engine Evas_Module_Engine;
struct _Evas_Module_Engine
{
   int 			id;
};

/* end of evas module api */
/**************************/

/* complain when peole pass in wrong object types etc. */
#define MAGIC_DEBUG

#define RENDER_METHOD_INVALID            0x00000000

typedef struct _Evas_Layer                  Evas_Layer;
typedef struct _Evas_Font_Dir               Evas_Font_Dir;
typedef struct _Evas_Font                   Evas_Font;
typedef struct _Evas_Font_Alias             Evas_Font_Alias;
typedef struct _Evas_Data_Node              Evas_Data_Node;
typedef struct _Evas_Func_Node              Evas_Func_Node;
typedef RGBA_Image_Loadopts                 Evas_Image_Load_Opts;
typedef struct _Evas_Func                   Evas_Func;
typedef struct _Evas_Image_Load_Func        Evas_Image_Load_Func;
typedef struct _Evas_Image_Save_Func        Evas_Image_Save_Func;
typedef struct _Evas_Object_Func            Evas_Object_Func;
typedef struct _Evas_Intercept_Func         Evas_Intercept_Func;
typedef struct _Evas_Intercept_Func_Basic   Evas_Intercept_Func_Basic;
typedef struct _Evas_Intercept_Func_SizePos Evas_Intercept_Func_SizePos;
typedef struct _Evas_Intercept_Func_Obj     Evas_Intercept_Func_Obj;
typedef struct _Evas_Intercept_Func_Int     Evas_Intercept_Func_Int;
typedef struct _Evas_Intercept_Func_Color   Evas_Intercept_Func_Color;
typedef struct _Evas_Key_Grab               Evas_Key_Grab;
typedef struct _Evas_Callbacks              Evas_Callbacks;
typedef struct _Evas_Format                 Evas_Format;

#define MAGIC_EVAS          0x70777770
#define MAGIC_OBJ           0x71777770
#define MAGIC_OBJ_RECTANGLE 0x71777771
#define MAGIC_OBJ_LINE      0x71777772
#define MAGIC_OBJ_GRADIENT  0x71777773
#define MAGIC_OBJ_POLYGON   0x71777774
#define MAGIC_OBJ_IMAGE     0x71777775
#define MAGIC_OBJ_TEXT      0x71777776
#define MAGIC_OBJ_SMART     0x71777777
#define MAGIC_OBJ_TEXTBLOCK 0x71777778
#define MAGIC_SMART         0x72777770

#ifdef MAGIC_DEBUG
#define MAGIC_CHECK_FAILED(o, t, m) \
{evas_debug_error(); \
 if (!o) evas_debug_input_null(); \
 else if (((t *)o)->magic == 0) evas_debug_magic_null(); \
 else evas_debug_magic_wrong((m), ((t *)o)->magic); \
}
#else
#define MAGIC_CHECK_FAILED(o, t, m)
#endif
#define MAGIC_CHECK(o, t, m) \
{if ((!o) || (!(((t *)o)->magic == (m)))) { \
MAGIC_CHECK_FAILED(o, t, m)
#define MAGIC_CHECK_END() \
}}

#define NEW_RECT(_r, _x, _y, _w, _h) \
{(_r) = malloc(sizeof(Evas_Rectangle)); \
if (_r) \
{ \
   (_r)->x = (_x); (_r)->y = (_y); \
   (_r)->w = (_w); (_r)->h = (_h); \
}}

#define MERR_NONE() _evas_alloc_error = EVAS_ALLOC_ERROR_NONE
#define MERR_FATAL() _evas_alloc_error = EVAS_ALLOC_ERROR_FATAL
#define MERR_BAD() _evas_alloc_error = EVAS_ALLOC_ERROR_RECOVERED

#define EVAS_OBJECT_IMAGE_FREE_FILE_AND_KEY(o)                              \
   if ((o)->cur.file)                                                       \
     {                                                                      \
         evas_stringshare_del((o)->cur.file);                               \
	 if ((o)->prev.file == (o)->cur.file)                               \
	       (o)->prev.file = NULL;                                       \
	 (o)->cur.file = NULL;                                              \
     }                                                                      \
   if ((o)->cur.key)                                                        \
     {                                                                      \
         evas_stringshare_del((o)->cur.key);                                \
	 if ((o)->prev.key == (o)->cur.key)                                 \
	       (o)->prev.key = NULL;                                        \
	 (o)->cur.key = NULL;                                               \
     }                                                                      \
   if ((o)->prev.file)                                                      \
     {                                                                      \
         evas_stringshare_del((o)->prev.file);                              \
	 (o)->prev.file = NULL;                                             \
     }                                                                      \
   if ((o)->prev.key)                                                       \
     {                                                                      \
         evas_stringshare_del((o)->prev.key);                               \
	 (o)->prev.key = NULL;                                              \
     }

struct _Evas_Intercept_Func_Basic
{
   void (*func) (void *data, Evas_Object *obj);
   void *data;
};

struct _Evas_Intercept_Func_SizePos
{
   void (*func) (void *data, Evas_Object *obj, Evas_Coord x, Evas_Coord y);
   void *data;
};

struct _Evas_Intercept_Func_Obj
{
   void (*func) (void *data, Evas_Object *obj, Evas_Object *obj2);
   void *data;
};

struct _Evas_Intercept_Func_Int
{
   void (*func) (void *data, Evas_Object *obj, int n);
   void *data;
};

struct _Evas_Intercept_Func_Color
{
   void (*func) (void *data, Evas_Object *obj, int r, int g, int b, int a);
   void *data;
};

struct _Evas_Key_Grab
{
   char               *keyname;
   Evas_Modifier_Mask  modifiers;
   Evas_Modifier_Mask  not_modifiers;
   Evas_Object        *object;
   unsigned char       exclusive : 1;
   unsigned char       just_added : 1;
   unsigned char       delete_me : 1;
};

struct _Evas_Intercept_Func
{
   Evas_Intercept_Func_Basic   show;
   Evas_Intercept_Func_Basic   hide;
   Evas_Intercept_Func_SizePos move;
   Evas_Intercept_Func_SizePos resize;
   Evas_Intercept_Func_Basic   raise;
   Evas_Intercept_Func_Basic   lower;
   Evas_Intercept_Func_Obj     stack_above;
   Evas_Intercept_Func_Obj     stack_below;
   Evas_Intercept_Func_Int     layer_set;
   Evas_Intercept_Func_Color   color_set;
   Evas_Intercept_Func_Obj     clip_set;
   Evas_Intercept_Func_Basic   clip_unset;
};

struct _Evas_Smart
{
   DATA32            magic;

   int               usage;

   const Evas_Smart_Class *smart_class;

   unsigned char     delete_me : 1;
   unsigned char     class_allocated : 1;

};

struct _Evas_Modifier
{
   struct {
      int       count;
      char    **list;
   } mod;
   Evas_Modifier_Mask mask; /* ok we have a max of 64 modifiers */
};

struct _Evas_Lock
{
   struct {
      int       count;
      char    **list;
   } lock;
   Evas_Modifier_Mask mask; /* we have a max of 64 locks */
};

struct _Evas_Callbacks
{
   Evas_Object_List *callbacks;
   int               walking_list;
   unsigned char     deletions_waiting : 1;
/*   
   Evas_Object_List *down;
   Evas_Object_List *up;
   Evas_Object_List *move;
   Evas_Object_List *in;
   Evas_Object_List *out;
   Evas_Object_List *wheel;
   Evas_Object_List *key_down;
   Evas_Object_List *key_up;
   Evas_Object_List *free;
   Evas_Object_List *obj_focus_in;
   Evas_Object_List *obj_focus_out;
   Evas_Object_List *obj_show;
   Evas_Object_List *obj_hide;
   Evas_Object_List *obj_move;
   Evas_Object_List *obj_resize;
   Evas_Object_List *obj_restack;
 */
};

struct _Evas
{
   Evas_Object_List  _list_data;

   DATA32            magic;

   struct {
      unsigned char  inside : 1;
      int            mouse_grabbed;
      DATA32         button;
      Evas_Coord     x, y;

////      Evas_Coord         canvas_x, canvas_y;

      struct {
	 Evas_List *in;
      } object;

   } pointer;

   struct  {
      Evas_Coord     x, y, w, h;
      unsigned char  changed : 1;
   } viewport;

   struct {
      int            w, h;
      DATA32         render_method;
      unsigned char  changed : 1;
   } output;

   Evas_List        *damages;
   Evas_List        *obscures;

   Evas_Layer       *layers;

   Evas_Hash        *name_hash;

   int               output_validity;

   int               walking_list;
   int               events_frozen;

   struct {
      Evas_Module *module;
      Evas_Func *func;
      struct {
	 void *output;

	 void *context;
      } data;

      void *info;
      int   info_magic;
   } engine;

   int            delete_grabs;
   int            walking_grabs;
   Evas_List     *grabs;

   Evas_List     *font_path;

   Evas_Object   *focused;
   void          *attach_data;
   Evas_Modifier  modifiers;
   Evas_Lock      locks;
   unsigned int   last_timestamp;
   int            last_mouse_down_counter;
   int            last_mouse_up_counter;
   Evas_Font_Hinting_Flags hinting;
   unsigned char     changed : 1;
   unsigned char  delete_me : 1;
};

struct _Evas_Layer
{
   Evas_Object_List  _list_data;

   int               layer;
   Evas_Object      *objects;

   Evas             *evas;

   void             *engine_data;
   int               usage;
   unsigned char     delete_me : 1;
};

struct _Evas_Object
{
   Evas_Object_List  _list_data;

   DATA32            magic;

   const char       *type;
   Evas_Layer       *layer;

   struct {
      struct {
/*	 
	 struct {
	    int            x, y, w, h;
	    int            validity;
	 } geometry;
 */
	 struct {
	    int            x, y, w, h;
	    unsigned char  r, g, b, a;
	    unsigned char  visible : 1;
	    unsigned char  dirty : 1;
	 } clip;
      } cache;
      struct {
	 Evas_Coord         x, y, w, h;
      } geometry;
      struct {
	 unsigned char  r, g, b, a;
      } color;
      unsigned char     visible : 1;
      unsigned char     have_clipees : 1;
      int               layer;
      Evas_Object      *clipper;
      unsigned char     anti_alias;
      struct {
         int color_space;
      } interpolation;
      int               render_op;
   } cur, prev;

   char                       *name;

   Evas_Intercept_Func *interceptors;

   struct {
      Evas_List *elements;
   } data;

   Evas_List *grabs;

   Evas_Callbacks *callbacks;

   struct {
      Evas_List   *clipees;
      Evas_List   *changes;
   } clip;

   const Evas_Object_Func *func;

   void             *object_data;

   struct {
      int               walking_list;
      Evas_Smart       *smart;
      void             *data;
      Evas_Object      *parent;
      Evas_Object_List *contained;
      Evas_List        *callbacks;
      unsigned char     deletions_waiting : 1;
   } smart;

   int                         last_mouse_down_counter;
   int                         last_mouse_up_counter;
   int                         mouse_grabbed;
   Evas_Object_Pointer_Mode    pointer_mode;

   unsigned short              store : 1;
   unsigned short              pass_events : 1;
   unsigned short              parent_pass_events : 1;
   unsigned short              parent_cache_valid : 1;
   unsigned short              repeat_events : 1;
   unsigned short              restack : 1;
   unsigned short              changed : 1;
   unsigned short              mouse_in : 1;
   unsigned short              pre_render_done : 1;
   unsigned short              intercepted : 1;
   unsigned short              focused : 1;
   unsigned short              in_layer : 1;
   unsigned short              no_propagate : 1;
   unsigned short              precise_is_inside : 1;

   unsigned char               delete_me;
};

struct _Evas_Func_Node
{
   Evas_Object_List  _list_data;
   void (*func) (void *data, Evas *e, Evas_Object *obj, void *event_info);
   void *data;
   Evas_Callback_Type type;
   unsigned char delete_me : 1;
};

struct _Evas_Data_Node
{
   char *key;
   void *data;
};

struct _Evas_Font_Dir
{
   Evas_Hash *lookup;
   Evas_List *fonts;
   Evas_List *aliases;
   DATA64     dir_mod_time;
   DATA64     fonts_dir_mod_time;
   DATA64     fonts_alias_mod_time;
};

struct _Evas_Font
{
   struct {
      const char *prop[14];
   } x;
   struct {
      const char *name;
   } simple;
   const char *path;
   char     type;
};

struct _Evas_Font_Alias
{
   const char *alias;
   Evas_Font  *fn;
};

struct _Evas_Object_Func
{
   void (*free) (Evas_Object *obj);
   void (*render) (Evas_Object *obj, void *output, void *context, void *surface, int x, int y);
   void (*render_pre) (Evas_Object *obj);
   void (*render_post) (Evas_Object *obj);

   void (*store) (Evas_Object *obj);
   void (*unstore) (Evas_Object *obj);

   int  (*is_visible) (Evas_Object *obj);
   int  (*was_visible) (Evas_Object *obj);

   int  (*is_opaque) (Evas_Object *obj);
   int  (*was_opaque) (Evas_Object *obj);

   int  (*is_inside) (Evas_Object *obj, Evas_Coord x, Evas_Coord y);
   int  (*was_inside) (Evas_Object *obj, Evas_Coord x, Evas_Coord y);

   void (*coords_recalc) (Evas_Object *obj);
};

struct _Evas_Func
{
   void *(*info)                           (Evas *e);
   void (*info_free)                       (Evas *e, void *info);
   void (*setup)                           (Evas *e, void *info);

   void (*output_free)                     (void *data);
   void (*output_resize)                   (void *data, int w, int h);
   void (*output_tile_size_set)            (void *data, int w, int h);
   void (*output_redraws_rect_add)         (void *data, int x, int y, int w, int h);
   void (*output_redraws_rect_del)         (void *data, int x, int y, int w, int h);
   void (*output_redraws_clear)            (void *data);
   void *(*output_redraws_next_update_get) (void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch);
   void (*output_redraws_next_update_push) (void *data, void *surface, int x, int y, int w, int h);
   void (*output_flush)                    (void *data);
   void (*output_idle_flush)               (void *data);

   void *(*context_new)                    (void *data);
   void (*context_free)                    (void *data, void *context);
   void (*context_clip_set)                (void *data, void *context, int x, int y, int w, int h);
   void (*context_clip_clip)               (void *data, void *context, int x, int y, int w, int h);
   void (*context_clip_unset)              (void *data, void *context);
   int  (*context_clip_get)                (void *data, void *context, int *x, int *y, int *w, int *h);
   void (*context_color_set)               (void *data, void *context, int r, int g, int b, int a);
   int  (*context_color_get)               (void *data, void *context, int *r, int *g, int *b, int *a);
   void (*context_multiplier_set)          (void *data, void *context, int r, int g, int b, int a);
   void (*context_multiplier_unset)        (void *data, void *context);
   int  (*context_multiplier_get)          (void *data, void *context, int *r, int *g, int *b, int *a);
   void (*context_cutout_add)              (void *data, void *context, int x, int y, int w, int h);
   void (*context_cutout_clear)            (void *data, void *context);
   void (*context_anti_alias_set)          (void *data, void *context, unsigned char aa);
   unsigned char (*context_anti_alias_get) (void *data, void *context);
   void (*context_color_interpolation_set) (void *data, void *context, int color_space);
   int  (*context_color_interpolation_get) (void *data, void *context);
   void (*context_render_op_set)           (void *data, void *context, int render_op);
   int  (*context_render_op_get)           (void *data, void *context);

   void (*rectangle_draw)                  (void *data, void *context, void *surface, int x, int y, int w, int h);

   void (*line_draw)                       (void *data, void *context, void *surface, int x1, int y1, int x2, int y2);

   void *(*polygon_point_add)              (void *data, void *context, void *polygon, int x, int y);
   void *(*polygon_points_clear)           (void *data, void *context, void *polygon);
   void (*polygon_draw)                    (void *data, void *context, void *surface, void *polygon);

   void *(*gradient_new)                   (void *data);
   void (*gradient_free)                   (void *data, void *gradient);
   void (*gradient_color_stop_add)         (void *data, void *gradient, int r, int g, int b, int a, int delta);
   void (*gradient_alpha_stop_add)         (void *data, void *gradient, int a, int delta);
   void (*gradient_color_data_set)         (void *data, void *gradient, void *map, int len, int alpha_flag);
   void (*gradient_alpha_data_set)         (void *data, void *gradient, void *alpha_map, int len);
   void (*gradient_clear)                  (void *data, void *gradient);
   void (*gradient_fill_set)               (void *data, void *gradient, int x, int y, int w, int h);
   void (*gradient_fill_angle_set)         (void *data, void *gradient, double fill_angle);
   void (*gradient_fill_spread_set)        (void *data, void *gradient, int spread);
   void (*gradient_angle_set)              (void *data, void *gradient, double angle);
   void (*gradient_offset_set)             (void *data, void *gradient, float offset);
   void (*gradient_direction_set)          (void *data, void *gradient, int direction);
   void (*gradient_type_set)               (void *data, void *gradient, char *name, char *params);
   int  (*gradient_is_opaque)              (void *data, void *context, void *gradient, int x, int y, int w, int h);
   int  (*gradient_is_visible)             (void *data, void *context, void *gradient, int x, int y, int w, int h);
   void (*gradient_render_pre)             (void *data, void *context, void *gradient);
   void (*gradient_render_post)            (void *data, void *gradient);
   void (*gradient_draw)                   (void *data, void *context, void *surface, void *gradient, int x, int y, int w, int h);

   void *(*image_load)                     (void *data, const char *file, const char *key, int *error, Evas_Image_Load_Opts *lo);
   void *(*image_new_from_data)            (void *data, int w, int h, DATA32 *image_data, int alpha, int cspace);
   void *(*image_new_from_copied_data)     (void *data, int w, int h, DATA32 *image_data, int alpha, int cspace);
   void (*image_free)                      (void *data, void *image);
   void (*image_size_get)                  (void *data, void *image, int *w, int *h);
   void *(*image_size_set)                 (void *data, void *image, int w, int h);
   void (*image_stride_get)                (void *data, void *image, int *stride);
   void *(*image_dirty_region)             (void *data, void *image, int x, int y, int w, int h);
   void *(*image_data_get)                 (void *data, void *image, int to_write, DATA32 **image_data);
   void *(*image_data_put)                 (void *data, void *image, DATA32 *image_data);
   void *(*image_alpha_set)                (void *data, void *image, int has_alpha);
   int  (*image_alpha_get)                 (void *data, void *image);
   void *(*image_border_set)               (void *data, void *image, int l, int r, int t, int b);
   void  (*image_border_get)               (void *data, void *image, int *l, int *r, int *t, int *b);
   void (*image_draw)                      (void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth);
   char *(*image_comment_get)              (void *data, void *image, char *key);
   char *(*image_format_get)               (void *data, void *image);
   void (*image_colorspace_set)            (void *data, void *image, int cspace);
   int  (*image_colorspace_get)            (void *data, void *image);
   void (*image_native_set)                (void *data, void *image, void *native);
   void *(*image_native_get)               (void *data, void *image);

   void (*image_cache_flush)               (void *data);
   void (*image_cache_set)                 (void *data, int bytes);
   int  (*image_cache_get)                 (void *data);

   void *(*font_load)                      (void *data, const char *name, int size);
   void *(*font_memory_load)               (void *data, char *name, int size, const void *fdata, int fdata_size);
   void *(*font_add)                       (void *data, void *font, const char *name, int size);
   void *(*font_memory_add)                (void *data, void *font, char *name, int size, const void *fdata, int fdata_size);
   void (*font_free)                       (void *data, void *font);
   int  (*font_ascent_get)                 (void *data, void *font);
   int  (*font_descent_get)                (void *data, void *font);
   int  (*font_max_ascent_get)             (void *data, void *font);
   int  (*font_max_descent_get)            (void *data, void *font);
   void (*font_string_size_get)            (void *data, void *font, const char *text, int *w, int *h);
   int  (*font_inset_get)                  (void *data, void *font, const char *text);
   int  (*font_h_advance_get)              (void *data, void *font, const char *text);
   int  (*font_v_advance_get)              (void *data, void *font, const char *text);
   int  (*font_char_coords_get)            (void *data, void *font, const char *text, int pos, int *cx, int *cy, int *cw, int *ch);
   int  (*font_char_at_coords_get)         (void *data, void *font, const char *text, int x, int y, int *cx, int *cy, int *cw, int *ch);
   void (*font_draw)                       (void *data, void *context, void *surface, void *font, int x, int y, int w, int h, int ow, int oh, const char *text);

   void (*font_cache_flush)                (void *data);
   void (*font_cache_set)                  (void *data, int bytes);
   int  (*font_cache_get)                  (void *data);

   /* Engine functions will over time expand from here */
   
   void (*font_hinting_set)                (void *data, void *font, int hinting);
   int  (*font_hinting_can_hint)           (void *data, int hinting);
   
/*    void (*image_rotation_set)              (void *data, void *image); */

};

struct _Evas_Image_Load_Func
{
  int (*file_head) (RGBA_Image *im, const char *file, const char *key);
  int (*file_data) (RGBA_Image *im, const char *file, const char *key);
};

struct _Evas_Image_Save_Func
{
  int (*image_save) (RGBA_Image *im, const char *file, const char *key, int quality, int compress);
};

#ifdef __cplusplus
extern "C" {
#endif

Evas_Object *evas_object_new(void);
void evas_object_free(Evas_Object *obj, int clean_layer);
void evas_object_inject(Evas_Object *obj, Evas *e);
void evas_object_release(Evas_Object *obj, int clean_layer);
void evas_object_change(Evas_Object *obj);
Evas_List *evas_object_render_pre_visible_change(Evas_List *updates, Evas_Object *obj, int is_v, int was_v);
Evas_List *evas_object_render_pre_clipper_change(Evas_List *updates, Evas_Object *obj);
Evas_List *evas_object_render_pre_prev_cur_add(Evas_List *updates, Evas_Object *obj);
void evas_object_render_pre_effect_updates(Evas_List *updates, Evas_Object *obj, int is_v, int was_v);
Evas_List * evas_rects_return_difference_rects(int x, int y, int w, int h, int xx, int yy, int ww, int hh);
void evas_object_clip_recalc(Evas_Object *obj);
void evas_object_clip_dirty(Evas_Object *obj);
void evas_object_recalc_clippees(Evas_Object *obj);
Evas_Layer *evas_layer_new(Evas *e);
void evas_layer_pre_free(Evas_Layer *lay);
void evas_layer_free(Evas_Layer *lay);
Evas_Layer *evas_layer_find(Evas *e, int layer_num);
void evas_layer_add(Evas_Layer *lay);
void evas_layer_del(Evas_Layer *lay);
void evas_object_coords_recalc(Evas_Object *obj);
int evas_object_is_active(Evas_Object *obj);
int evas_object_is_in_output_rect(Evas_Object *obj, int x, int y, int w, int h);
int evas_object_was_in_output_rect(Evas_Object *obj, int x, int y, int w, int h);
int evas_object_is_visible(Evas_Object *obj);
int evas_object_was_visible(Evas_Object *obj);
int evas_object_is_opaque(Evas_Object *obj);
int evas_object_was_opaque(Evas_Object *obj);
int evas_object_is_inside(Evas_Object *obj, Evas_Coord x, Evas_Coord y);
int evas_object_was_inside(Evas_Object *obj, Evas_Coord x, Evas_Coord y);
//void evas_object_recalc_clippees(Evas_Object *obj);
int evas_object_clippers_is_visible(Evas_Object *obj);
int evas_object_clippers_was_visible(Evas_Object *obj);
void evas_object_event_callback_call(Evas_Object *obj, Evas_Callback_Type type, void *event_info);
int evas_event_passes_through(Evas_Object *obj);
Evas_List *evas_event_objects_event_list(Evas *e, Evas_Object *stop, int x, int y);
int evas_file_path_is_full_path(const char *path);
char *evas_file_path_join(const char *path, const char *end);
int evas_file_path_exists(const char *path);
int evas_file_path_is_file(const char *path);
int evas_file_path_is_dir(const char *path);
Evas_List *evas_file_path_list(char *path, const char *match, int match_case);
DATA64 evas_file_modified_time(const char *file);
char *evas_file_path_resolve(const char *file);
int evas_mem_free(int mem_required);
int evas_mem_degrade(int mem_required);
void evas_debug_error(void);
void evas_debug_input_null(void);
void evas_debug_magic_null(void);
void evas_debug_magic_wrong(DATA32 expected, DATA32 supplied);
void evas_debug_generic(const char *str);
const char *evas_debug_magic_string_get(DATA32 magic);
void evas_object_smart_use(Evas_Smart *s);
void evas_object_smart_unuse(Evas_Smart *s);
void evas_object_smart_del(Evas_Object *obj);
void evas_object_smart_cleanup(Evas_Object *obj);
void *evas_mem_calloc(int size);
void evas_object_event_callback_all_del(Evas_Object *obj);
void evas_object_event_callback_cleanup(Evas_Object *obj);
void evas_object_inform_call_show(Evas_Object *obj);
void evas_object_inform_call_hide(Evas_Object *obj);
void evas_object_inform_call_move(Evas_Object *obj);
void evas_object_inform_call_resize(Evas_Object *obj);
void evas_object_inform_call_restack(Evas_Object *obj);
void evas_object_intercept_cleanup(Evas_Object *obj);
int evas_object_intercept_call_show(Evas_Object *obj);
int evas_object_intercept_call_hide(Evas_Object *obj);
int evas_object_intercept_call_move(Evas_Object *obj, Evas_Coord x, Evas_Coord y);
int evas_object_intercept_call_resize(Evas_Object *obj, Evas_Coord w, Evas_Coord h);
int evas_object_intercept_call_raise(Evas_Object *obj);
int evas_object_intercept_call_lower(Evas_Object *obj);
int evas_object_intercept_call_stack_above(Evas_Object *obj, Evas_Object *above);
int evas_object_intercept_call_stack_below(Evas_Object *obj, Evas_Object *below);
int evas_object_intercept_call_layer_set(Evas_Object *obj, int l);
int evas_object_intercept_call_color_set(Evas_Object *obj, int r, int g, int b, int a);
int evas_object_intercept_call_clip_set(Evas_Object *obj, Evas_Object *clip);
int evas_object_intercept_call_clip_unset(Evas_Object *obj);
void evas_object_grabs_cleanup(Evas_Object *obj);
void evas_key_grab_free(Evas_Object *obj, const char *keyname, Evas_Modifier_Mask modifiers, Evas_Modifier_Mask not_modifiers);
void evas_font_dir_cache_free(void);
const char *evas_font_dir_cache_find(char *dir, char *font);
Evas_List *evas_font_dir_available_list(const Evas* evas);
void evas_font_dir_available_list_free(Evas_List *available);
void evas_font_free(Evas *evas, void *font);
void *evas_font_load(Evas *evas, const char *name, const char *source, int size);
void evas_font_load_hinting_set(Evas *evas, void *font, int hinting);   
void evas_object_smart_member_cache_invalidate(Evas_Object *obj);
void evas_text_style_pad_get(Evas_Text_Style_Type style, int *l, int *r, int *t, int *b);
void _evas_object_text_rehint(Evas_Object *obj);
void _evas_object_textblock_rehint(Evas_Object *obj);
       
extern int _evas_alloc_error;

struct _Evas_Imaging_Image
{
   RGBA_Image *image;
};

struct _Evas_Imaging_Font
{
   RGBA_Font *font;
};

typedef struct _Evas_Mempool Evas_Mempool;
   
struct _Evas_Mempool
{
   int           item_size;
   int           pool_size;
   int           usage;
   void         *first, *last;
};

void *evas_mempool_malloc(Evas_Mempool *pool, int size);
void  evas_mempool_free(Evas_Mempool *pool, void *ptr);
void *evas_mempool_calloc(Evas_Mempool *pool, int size);

void evas_module_paths_init(void);
void evas_module_init(void);
Evas_Module * evas_module_find_type(Evas_Module_Type type, const char *name);
int evas_module_load(Evas_Module *em);
void evas_module_unload(Evas_Module *em);
void evas_module_ref(Evas_Module *em);
void evas_module_unref(Evas_Module *em);
void evas_module_use(Evas_Module *em);
void evas_module_clean(void);
void evas_module_shutdown(void);

void _evas_walk(Evas *e);
void _evas_unwalk(Evas *e);
       
EAPI int _evas_module_engine_inherit(Evas_Func *funcs, char *name);
       
#define EVAS_API_OVERRIDE(func, api, prefix) \
     (api)->func = prefix##func
#ifdef __cplusplus
}
#endif
#endif
