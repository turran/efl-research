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

#ifndef ETK2_RENDERABLE_H_
#define ETK2_RENDERABLE_H_

typedef struct _Renderable_Private Renderable_Private;

struct _Renderable
{
	Object parent;
	/* Function called whenever the canvas needs this renderable
	 * to be rendered
	 */
	void (*render)(Renderable *r, Eina_Rectangle *rect);
	Renderable_Private *private;
};

Type *renderable_type_get(void);
EAPI void renderable_geometry_set(Renderable *r, Eina_Rectangle *rect);
EAPI void renderable_geometry_get(Renderable *r, Eina_Rectangle *rect);
EAPI Canvas * renderable_canvas_get(Renderable *r);
EAPI void renderable_show(Renderable *r);
EAPI void renderable_hide(Renderable *r);
EAPI void renderable_visibility_set(Renderable *r, Eina_Bool visible);
EAPI void renderable_visibility_get(Renderable *r, Eina_Bool *visible);

#endif /* ETK2_RENDERABLE_H_ */
