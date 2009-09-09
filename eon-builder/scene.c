#include "builder.h"

/* TODO
 * add an area selector
 */
static void _scene_click_cb(const Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	Scene *sc = (Scene *)data;
	Eina_List *l;

	for (l = sc->selected; l; l = eina_list_next(l))
	{
		Shape *s = eina_list_data_get(l);

		shape_state_set(s, NONE);
	}
}

void scene_selected_remove(Scene *s, Shape *sh)
{
	s->selected = eina_list_remove(s->selected, sh);

}

void scene_selected_add(Scene *s, Shape *sh)
{
	s->selected = eina_list_append(s->selected, sh);
}

Scene * scene_new(Eon_Canvas *canvas)
{
	Eon_Rect *r;
	Scene *sc;

	sc = malloc(sizeof(Scene));
	sc->selected = NULL;
	sc->canvas = canvas;

	/* TODO add an event listener on the background */
	r = eon_rect_new(canvas);
	eon_rect_color_set(r, 0xffffffff);
	eon_shape_draw_mode_set((Eon_Shape *)r, ENESIM_SHAPE_DRAW_MODE_FILL);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_rect_show(r);
	ekeko_event_listener_add(r, EKEKO_EVENT_UI_MOUSE_CLICK, _scene_click_cb, EINA_FALSE, sc);

	sc->sh = (Eon_Shape *)r;
	return sc;
}
