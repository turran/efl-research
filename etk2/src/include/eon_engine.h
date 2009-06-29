#ifndef EON_ENGINE_H_
#define EON_ENGINE_H_

/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
#if 0
typedef struct _Eon_Document_Engine
{
	void * (*create)(Eon_Document *);
	void (*delete)(Eon_Document *, void *);
} Eon_Document_Engine;

typedef struct _Eon_Canvas_Engine
{
	void (*lock)(void *s);
	void (*unlock)(void *s);
	void * (*create)(Eina_Bool root, int w, int h);
	void (*blit)(void *s, void *context, void *src, Eina_Rectangle *srect);
	Eina_Bool (*flush)(void *s, Eina_Rectangle *srect);
} Eon_Canvas_Engine;

typedef struct _Eon_Context_Engine
{
	/* basic operations */
	void * (*create)(void);
	void (*delete)(void *context);
	/* shape properties */
	void (*color_set)(void *context, int color); /* FIXME change this int */
	void (*rop_set)(void *context, int rop); /* FIXME change this int */
	/* transformations */
	void (*matrix_set)(void *c, Enesim_Matrix *m);
	/* clipping */
	void (*clip_set)(void *c, Eina_Rectangle *r);
	void (*clip_clear)(void *c);
	/* filters */
} Eon_Context_Engine;

typedef struct _Eon_Shape_Engine
{
	void (*rect)(void *surface, void *context, int x, int y, int w, int h);
	void (*image)(void *surface, void *context, Enesim_Surface *s, Eina_Rectangle *srect);
	void *(*polygon_new)(void);
	void (*polygon_point_add)(void *polygon, int x, int y);
	void (*polygon_render)(void *surface, void *context, void *polygon);
} Eon_Shape_Engine;

struct _Eon_Engine
{
	Eon_Document_Engine *doc;
	Eon_Canvas_Engine *canvas;
	Eon_Context_Engine *context;
	Eon_Shape_Engine *shape;
};
#endif
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Engine_Private Eon_Engine_Private;
struct _Eon_Engine
{
	Ekeko_Object parent;
	Eon_Engine_Private *private;
	/* document callbacks */
	void * (*document_create)(Eon_Document *d);
	void (*document_delete)(void *);
	/* canvas callbacks */
	void * (*canvas_create)(Eon_Canvas *c, Eina_Bool root, int w, int h);
	void (*canvas_delete)(void *);
	Eina_Bool (*canvas_blit)(void *sc, Eina_Rectangle *r, void *c, Eina_Rectangle *sr);
	Eina_Bool (*canvas_flush)(void *c, Eina_Rectangle *r);
	/* rect callbacks */
	void * (*rect_create)(Eon_Rect *r);
	void (*rect_render)(void *r, void *c, Eina_Rectangle *clip);
	/* circle callbacks */
	void * (*circle_create)(Eon_Circle *r);
	void (*circle_render)(void *r, void *c, Eina_Rectangle *clip);
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
typedef Ekeko_Type * (*Eon_Engine_New)(void);
EAPI Ekeko_Type *eon_engine_type_get(void);

EAPI void * eon_engine_document_create(Eon_Engine *e, Eon_Document *d);

EAPI Eina_Bool eon_engine_canvas_blit(Eon_Engine *e, void *sc, Eina_Rectangle *r, void *c, Eina_Rectangle *sr);
EAPI Eina_Bool eon_engine_canvas_flush(Eon_Engine *e, void *c, Eina_Rectangle *r);
EAPI void * eon_engine_canvas_create(Eon_Engine *e, Eon_Canvas *c, Eina_Bool root, uint32_t w, uint32_t h);

EAPI void * eon_engine_rect_create(Eon_Engine *e, Eon_Rect *r);
EAPI void eon_engine_rect_render(Eon_Engine *e, void *r, void *c, Eina_Rectangle *clip);

EAPI void * eon_engine_circle_create(Eon_Engine *e, Eon_Circle *c);
EAPI void eon_engine_circle_render(Eon_Engine *e, void *r, void *c, Eina_Rectangle *clip);

#endif /* EON_ENGINE_H_ */
