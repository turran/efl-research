#include "builder.h"

static _shape_show(Shape *s)
{
	eon_rect_show(s->area);
	if (s->state == SCALE || s->state == ROTATE)
	{
		int i;
		for (i = 0; i < CONTROL_POSITIONS; i++)
			control_show(s->cp[i]);
	}
}

static _shape_hide(Shape *s)
{
	int i;

	for (i = 0; i < CONTROL_POSITIONS; i++)
		control_hide(s->cp[i]);
	eon_rect_hide(s->area);
}

static void _shape_update_geom(Shape *s)
{
	Eina_Rectangle geom;
	int i;

	ekeko_renderable_geometry_get((Ekeko_Renderable *)s->sh, &geom);
	eon_rect_x_set(s->area, geom.x - 4);
	eon_rect_y_set(s->area, geom.y - 4);
	eon_rect_w_set(s->area, geom.w + 8);
	eon_rect_h_set(s->area, geom.h + 8);

	if (s->transforming)
		return;
	for (i = 0; i < CONTROL_POSITIONS; i++)
		control_update(s->cp[i]);
}

static void _shape_move_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mouse *ev = (Ekeko_Event_Mouse *)e;
	Shape *s = (Shape *)data;
	Eina_List *l;
	int dx, dy;

	dx = ev->screen.x - ev->screen.prev_x;
	dy = ev->screen.y - ev->screen.prev_y;

	/* check the key modifiers */
	/* just clear the selected objects */
	//scene_selected_clear(s->sc);
	if (s->state == NONE)
		scene_selected_add(s->sc, s);

	s->state = SCALE;
	_shape_show(s);
	/* move every scene's selected object */
	for (l = s->sc->selected; l; l = eina_list_next(l))
	{
		Shape *sl = eina_list_data_get(l);

		if (sl->smove)
			sl->smove(sl, dx, dy);
	}
}

static void _shape_down_cb(const Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ekeko_event_listener_add(o, EKEKO_EVENT_UI_MOUSE_MOVE, _shape_move_cb, EINA_FALSE, data);
}

static void _shape_up_cb(const Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ekeko_event_listener_remove(o, EKEKO_EVENT_UI_MOUSE_MOVE, _shape_move_cb, EINA_FALSE, data);
}

static void _shape_click_cb(const Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	Shape *s = data;
	Shape_State st;

	st = (s->state + 1) % SHAPE_STATES;
	shape_state_set(s, st);
}

static void _shape_geom_cb(const Ekeko_Object *o, Ekeko_Event *e, void * data)
{
	Shape *s = data;
	Eina_Rectangle geom;
	Ekeko_Event_Mutation *ev = e;

	if (ev->state != EVENT_MUTATION_STATE_CURR)
		return;
	_shape_update_geom(s);
}

void shape_state_set(Shape *s, Shape_State st)
{
	Shape_State prev;

	prev = s->state;
	s->state = st;
	switch (st)
	{
		case SCALE:
		case ROTATE:
		case PROJECT:
		if (prev == NONE)
		{
			_shape_show(s);
			scene_selected_add(s->sc, s);
		}
		break;

		case NONE:
		if (prev != NONE)
		{
			scene_selected_remove(s->sc, s);
			_shape_hide(s);
		}
		break;
	}
}

Shape * shape_new(Eon_Shape *es, Scene *sc, Control_Move cmove,
		Shape_Move smove)
{
	Eon_Canvas *c;
	Shape *s;
	int i;

	s = malloc(sizeof(Shape));

	c = eon_shape_canvas_get(es);
	s->area = eon_rect_new(c);
	s->sh = es;
	s->state = NONE;
	s->transforming = EINA_FALSE;
	s->sc = sc;
	s->angle = 0;

	/* the border */
	eon_shape_stroke_color_set(s->area, 0xff000000);
	eon_shape_stroke_width_set(s->area, 2);
	eon_shape_draw_mode_set(s->area, ENESIM_SHAPE_DRAW_MODE_STROKE);
	eon_shape_rop_set(s->area, ENESIM_BLEND);
	/* the control points */
	for (i = 0; i < CONTROL_POSITIONS; i++)
	{
		s->cp[i] = control_new(s, i);
	}
	_shape_update_geom(s);

	/* register the needed callbacks */
	ekeko_event_listener_add(es, EKEKO_EVENT_UI_MOUSE_CLICK, _shape_click_cb, EINA_FALSE, s);
	ekeko_event_listener_add(es, EKEKO_EVENT_UI_MOUSE_DOWN, _shape_down_cb, EINA_FALSE, s);
	ekeko_event_listener_add(es, EKEKO_EVENT_UI_MOUSE_UP, _shape_up_cb, EINA_FALSE, s);
	ekeko_event_listener_add(es, EKEKO_RENDERABLE_GEOMETRY_CHANGED, _shape_geom_cb, EINA_FALSE, s);

	s->cmove = cmove;
	s->smove = smove;

	return s;
}


