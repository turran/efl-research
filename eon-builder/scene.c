#include "builder.h"

void scene_selected_remove(Scene *s, Shape *sh)
{

}

void scene_selected_add(Scene *s, Shape *sh)
{

}

Scene * scene_new(Eon_Canvas *canvas)
{
	Eon_Rect *r;
	Scene *s = malloc(sizeof(Scene));

	/* TODO add an event listener on the background */
	r = eon_rect_new(canvas);
	eon_rect_color_set(r, 0xffffffff);
	eon_shape_draw_mode_set((Eon_Shape *)r, ENESIM_SHAPE_DRAW_MODE_FILL);
	eon_rect_x_rel_set(r, 0);
	eon_rect_y_rel_set(r, 0);
	eon_rect_w_rel_set(r, 100);
	eon_rect_h_rel_set(r, 100);
	eon_rect_show(r);

	s->sh = (Eon_Shape *)r;
	return s;
}
