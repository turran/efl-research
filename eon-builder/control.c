#include "builder.h"

static void _control_move_cb(const Ekeko_Object *o, Ekeko_Event *e, void * data)
{
	Ekeko_Event_Mouse *ev = (Ekeko_Event_Mouse *)e;
	Control *cp = data;
	Shape *s = cp->ref;
	int dx, dy;

	dx = ev->screen.x - ev->screen.prev_x;
	dy = ev->screen.y - ev->screen.prev_y;

	if (s->state == SCALE)
	{
		if (s->cmove)
			s->cmove(cp, dx, dy);
	}
	else if (s->state == ROTATE)
	{
		Enesim_Matrix m;
		Enesim_Matrix tmp;

		eon_shape_matrix_get(s->sh, &m);
		switch (cp->pos)
		{
			case TOP_LEFT:
			s->angle += (dx * (M_PI_4 / 10.0));
			break;

		}
		printf("angle = %g\n", s->angle);
		enesim_matrix_rotate(&tmp, s->angle);
		//enesim_matrix_compose(&m, &m, &tmp);
		eon_shape_matrix_set(s->sh, &tmp);
	}
}

static void _control_down_cb(const Ekeko_Object *o, Ekeko_Event *e, void * data)
{
	Control *cp = data;
	Shape *s = cp->ref;

	ekeko_event_listener_add(o, EKEKO_EVENT_UI_MOUSE_MOVE, _control_move_cb, EINA_FALSE, cp);
}

static void _control_up_cb(const Ekeko_Object *o, Ekeko_Event *e, void * data)
{
	Control *cp = data;

	ekeko_event_listener_remove(o, EKEKO_EVENT_UI_MOUSE_MOVE, _control_move_cb, EINA_FALSE, cp);
	//_shape_update_cp(data);
}

Control * control_new(Shape *ref, Control_Position pos)
{
	Eon_Canvas *c;
	Control *cp;

	c = eon_shape_canvas_get(ref->sh);
	cp = malloc(sizeof(Control));
	cp->ref = ref;
	cp->pos = pos;
	cp->sh = eon_rect_new(c);

	eon_shape_color_set(cp->sh, 0xff000000);
	eon_shape_draw_mode_set(cp->sh, ENESIM_SHAPE_DRAW_MODE_FILL);
	eon_rect_w_set(cp->sh, 5);
	eon_rect_h_set(cp->sh, 5);

	/* events */
	ekeko_event_listener_add(cp->sh, EKEKO_EVENT_UI_MOUSE_DOWN, _control_down_cb, EINA_FALSE, cp);
	ekeko_event_listener_add(cp->sh, EKEKO_EVENT_UI_MOUSE_UP, _control_up_cb, EINA_FALSE, cp);

	return cp;
}

void control_show(Control *cp)
{
	eon_shape_show(cp->sh);
}

void control_hide(Control *cp)
{
	eon_shape_hide(cp->sh);
}

void control_update(Control *cp)
{
	Eina_Rectangle geom;
	Eina_Rectangle cgeom;
	const int off = 2;

	ekeko_renderable_geometry_get((Ekeko_Renderable *)cp->ref->area, &geom);
	ekeko_renderable_geometry_get((Ekeko_Renderable *)cp->sh, &cgeom);
	switch (cp->pos)
	{
		case TOP_LEFT:
		eon_rect_x_set(cp->sh, geom.x - (cgeom.w + off));
		eon_rect_y_set(cp->sh, geom.y - (cgeom.h + off));
		break;

		case TOP_MIDDLE:
		eon_rect_x_set(cp->sh, geom.x + (geom.w / 2) - (cgeom.w / 2));
		eon_rect_y_set(cp->sh, geom.y - (cgeom.h + off));
		break;

		case TOP_RIGHT:
		eon_rect_x_set(cp->sh, geom.x + geom.w + off);
		eon_rect_y_set(cp->sh, geom.y - (cgeom.h + off));
		break;

		case CENTER_LEFT:
		eon_rect_x_set(cp->sh, geom.x - (cgeom.h + off));
		eon_rect_y_set(cp->sh, geom.y + (geom.h / 2) - off);
		break;

		case CENTER_RIGHT:
		eon_rect_x_set(cp->sh, geom.x + geom.w + 5);
		eon_rect_y_set(cp->sh, geom.y + (geom.h / 2) - off);
		break;

		case BOTTOM_LEFT:
		eon_rect_x_set(cp->sh, geom.x - (cgeom.w + off));
		eon_rect_y_set(cp->sh, geom.y + geom.h + (cgeom.h + off));
		break;

		case BOTTOM_MIDDLE:
		eon_rect_x_set(cp->sh, geom.x + (geom.w / 2) - (cgeom.w / 2));
		eon_rect_y_set(cp->sh, geom.y + geom.h + (cgeom.h + off));
		break;


		case BOTTOM_RIGHT:
		eon_rect_x_set(cp->sh, geom.x + geom.w + 5);
		eon_rect_y_set(cp->sh, geom.y + geom.h + (cgeom.h + off));
		break;

	}
#if 0
	/* cm */
	eon_rect_x_set(s->cp[4], geom.x + (geom.w / 2) - 3);
	eon_rect_y_set(s->cp[4], geom.y + (geom.h / 2) - 3);
#endif
}


