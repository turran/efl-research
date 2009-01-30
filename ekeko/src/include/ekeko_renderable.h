/*
 * etk2_renderable.h
 *
 *  Created on: 12-ene-2009
 *      Author: jl
 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

#ifndef EKEKO_RENDERABLE_H_
#define EKEKO_RENDERABLE_H_

typedef struct _Ekeko_Renderable_Private Ekeko_Renderable_Private;

struct _Ekeko_Renderable
{
	Ekeko_Object parent;
	/* Function called whenever the canvas needs this renderable
	 * to be rendered
	 */
	void (*render)(Ekeko_Renderable *r, Eina_Rectangle *rect);
	Ekeko_Renderable_Private *private;
};

Ekeko_Type *ekeko_renderable_type_get(void);
EAPI void ekeko_renderable_geometry_set(Ekeko_Renderable *r, Eina_Rectangle *rect);
EAPI void ekeko_renderable_geometry_get(Ekeko_Renderable *r, Eina_Rectangle *rect);
EAPI Ekeko_Canvas * ekeko_renderable_canvas_get(Ekeko_Renderable *r);
EAPI void ekeko_renderable_show(Ekeko_Renderable *r);
EAPI void ekeko_renderable_hide(Ekeko_Renderable *r);
EAPI void ekeko_renderable_visibility_set(Ekeko_Renderable *r, Eina_Bool visible);
EAPI void ekeko_renderable_visibility_get(Ekeko_Renderable *r, Eina_Bool *visible);
EAPI void ekeko_renderable_move(Ekeko_Renderable *r, int x, int y);
EAPI void ekeko_renderable_resize(Ekeko_Renderable *r, int w, int h);

#endif /* EKEKO_RENDERABLE_H_ */
