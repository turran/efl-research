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

void _sdc_down(const Object *o, Event *e)
{
	printf("Mouse down\n");
}

void _sdc_up(const Object *o, Event *e)
{
	printf("Mouse up\n");
}

void _sdc_click(const Object *o, Event *e)
{
	printf("Mouse click\n");
}

void _rect2_over(const Object *o, Event *e)
{
	/* hide o show rect1 */
	renderable_hide((Renderable *)o);
	renderable_show((Renderable *)rect1);
}

void _subrect_over(const Object *o, Event *e)
{
	printf("MOUSE IN RECTANGLE INSIDE A SUBCANVAS\n");
}

int main(void)
{
	Dummy *d;
	Dummy_Canvas *dc;
	Dummy_Canvas *sdc;
	Dummy_Rect *dr;

	dummy_init();
	d = dummy_new();
	/* canvas */
	dc = dummy_canvas_get(d);
	dummy_canvas_resize(dc, 320, 240);
#if 1
	/* background */
	dr = dummy_rect_add(dc);
	dummy_rect_move(dr, 0, 0);
	dummy_rect_resize(dr, 320, 240);
	dummy_rect_color_set(dr, 0xffffffff);
	renderable_show((Renderable *)dr);
#if 1
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
#endif
#endif
	/* another canvas */
	sdc = dummy_canvas_new();
	object_child_append((Object *)dc, (Object *)sdc);
	renderable_move((Renderable *)sdc, 100, 100);
	renderable_resize((Renderable *)sdc, 150, 150);
	renderable_show((Renderable *)sdc);
	event_listener_add((Object *)sdc, EVENT_UI_MOUSE_DOWN, _sdc_down, EINA_FALSE);
	event_listener_add((Object *)sdc, EVENT_UI_MOUSE_UP, _sdc_up, EINA_FALSE);
	event_listener_add((Object *)sdc, EVENT_UI_MOUSE_CLICK, _sdc_click, EINA_FALSE);
#if 1
	dr = dummy_rect_add(sdc);
	dummy_rect_move(dr, 0, 0);
	dummy_rect_resize(dr, 50, 50);
	dummy_rect_color_set(dr, 0x00ffffff);
	renderable_show((Renderable *)dr);
	event_listener_add((Object *)dr, EVENT_UI_MOUSE_IN, _subrect_over, EINA_FALSE);
#endif
	/* the looooop :) */
	dummy_loop();
	dummy_shutdown();

	return 0;
}
