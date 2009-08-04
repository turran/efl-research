#include "eranoid.h"

static Ekeko_Type *ball_type_get(void);
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Ball_Private *)((Ball *)(d))->prv)

struct _Ball_Private
{
	Eon_Rect *shape;
	int ix;
	int iy;
	float v;
};

static void _ctor(void *instance)
{
	Ball *b;
	Ball_Private *prv;

	b = (Ball *)instance;
	b->prv = prv = ekeko_type_instance_private_get(ball_type_get(), instance);
	prv->ix = 1;
	prv->iy = 1;
	prv->v = 1;
}

static void _dtor(void *polygon)
{

}

static Ekeko_Type *ball_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_BALL, sizeof(Ball),
				sizeof(Ball_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

static void ball_pos_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	BallEvent be;
	Ball *b = data;
	Ball_Private *prv = PRIVATE(b);
	Eon_Coord x, y, w, h;

	eon_square_coords_get((Eon_Square *)prv->shape, &x, &y, &w, &h);
	ekeko_event_init((Ekeko_Event *)&be, BALL_EVENT, data, EINA_FALSE);
	be.x = x.final;
	be.y = y.final;
	be.w = w.final;
	be.h = h.final;
	ekeko_event_dispatch((Ekeko_Event *)&be);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ball * ball_new(Eon_Canvas *c)
{
	Ball *b;
	Ball_Private *prv;
	Eon_Rect *s;

	b = ekeko_type_instance_new(ball_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)b);
	s = eon_rect_new(c);

	eon_rect_x_set(s, 50);
	eon_rect_y_set(s, 30);
	eon_rect_w_set(s, 10);
	eon_rect_h_set(s, 10);
	eon_rect_color_set(s, 0xff000000);
	eon_rect_show(s);
	prv = PRIVATE(b);
	prv->shape = s;

	/* we'll always change both attributes, xand y at once, we should only listen
	 * to one attribute
	 */
	ekeko_event_listener_add((Ekeko_Object *)s, EON_SQUARE_X_CHANGED, ball_pos_change, EINA_FALSE, b);

	return b;
}

void ball_update(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);
	Eon_Coord x, y;

	eon_square_coords_get((Eon_Square *)prv->shape, &x, &y, NULL, NULL);
	eon_rect_x_set(prv->shape, x.value + (prv->v * prv->ix));
	eon_rect_y_set(prv->shape, y.value + (prv->v * prv->iy));
}

void ball_bounce_x(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);

	prv->ix = -prv->ix;
}

void ball_bounce_y(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);

	prv->iy = -prv->iy;
}


void ball_vel_set(Ball *b, float vel)
{
	Ball_Private *prv = PRIVATE(b);

	prv->v = vel;
}

void ball_pos_set(Ball *b, int x, int y)
{
	Ball_Private *prv = PRIVATE(b);

	eon_rect_x_set(prv->shape, x);
	eon_rect_y_set(prv->shape, y);
}
