/*
 * renderable.h
 *
 *  Created on: 30-ene-2009
 *      Author: jl
 */
#ifndef RENDERABLE_H_
#define RENDERABLE_H_

Eina_Bool renderable_appended_get(Ekeko_Renderable *r);
void renderable_appended_set(Ekeko_Renderable *r, Eina_Bool appended);
int ekeko_renderable_zindex_get(Ekeko_Renderable *r);
void ekeko_renderable_zindex_set(Ekeko_Renderable *r, int zindex);
Eina_Bool ekeko_renderable_intersect(Ekeko_Renderable *r, int x, int y);

#endif /* RENDERABLE_H_ */
