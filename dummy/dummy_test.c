/*
 * dummy_test.c
 *
 *  Created on: 15-ene-2009
 *      Author: jl
 */
#include "dummy.h"

/* TODO avoid this and pass those as parameters */
Dummy_Rect *rect1;
Dummy_Rect *rect2;
Dummy_Rect *bkg;

void _bkg_resize(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;
	if (!strcmp(em->prop, "geometry"))
	{
		printf("Changing background GEOMETRY!!!\n");
		renderable_geometry_set((Renderable *)bkg, &em->curr->value.rect);
	}
}

void _rect1_over(const Object *o, Event *e)
{
	/* hide o show rect2 */
	renderable_hide((Renderable *)o);
	renderable_show((Renderable *)rect2);
}

void _rect2_over(const Object *o, Event *e)
{
	/* hide o show rect1 */
	renderable_hide((Renderable *)o);
	renderable_show((Renderable *)rect1);
}

int main(void)
{
	Dummy *d;
	Dummy_Canvas *dc;
	Dummy_Rect *dr;

	dummy_init();
	d = dummy_new();
	/* canvas */
	dc = dummy_canvas_get(d);
	dummy_canvas_resize(dc, 320, 240);
	/* background */
	dr = dummy_rect_add(dc);
	dummy_rect_move(dr, 0, 0);
	dummy_rect_resize(dr, 320, 240);
	dummy_rect_color_set(dr, 0xffffffff);
	renderable_show((Renderable *)dr);
	/* whenever the canvas resize also resize the background */
	event_listener_add((Object *)dc, EVENT_PROP_MODIFY, _bkg_resize, EINA_FALSE);
	bkg = dr;
	/* rect 1 */
	dr = dummy_rect_add(dc);
	dummy_rect_move(dr, 20, 20);
	dummy_rect_resize(dr, 100, 100);
	dummy_rect_color_set(dr, 0xff00ff00);
	renderable_show((Renderable *)dr);
	rect1 = dr;
	event_listener_add((Object *)rect1, EVENT_UI_MOUSE_IN, _rect1_over, EINA_FALSE);
	/* rect 2 */
	dr = type_instance_new(dummy_rect_type_get());
	dummy_rect_move(dr, 120, 120);
	dummy_rect_resize(dr, 100, 100);
	dummy_rect_color_set(dr, 0xffff0000);
	object_child_append((Object *)dc, (Object *)dr);
	renderable_hide((Renderable *)dr);
	rect2 = dr;
	event_listener_add((Object *)rect2, EVENT_UI_MOUSE_IN, _rect2_over, EINA_FALSE);
	/* the looooop :) */
	dummy_loop();
	dummy_shutdown();

	return 0;
}
