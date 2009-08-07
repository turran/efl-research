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
	float m;
	float error;
};

static void _ctor(void *instance)
{
	Ball *b;
	Ball_Private *prv;

	b = (Ball *)instance;
	b->prv = prv = ekeko_type_instance_private_get(ball_type_get(), instance);
	prv->ix = 1;
	prv->iy = 1;
	prv->m = 1;
	prv->error = 0;
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
	ekeko_object_event_dispatch(data, (Ekeko_Event *)&be);
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

	eon_rect_x_set(s, 105);
	eon_rect_y_set(s, 80);
	eon_rect_w_set(s, 10);
	eon_rect_h_set(s, 10);
	eon_rect_color_set(s, 0xffff0000);
	eon_rect_show(s);
	prv = PRIVATE(b);
	prv->shape = s;

	/* we'll always change both attributes, x and y at once, we should only listen
	 * to one attribute
	 */
	ekeko_event_listener_add((Ekeko_Object *)s, EON_SQUARE_X_CHANGED, ball_pos_change, EINA_FALSE, b);

	return b;
}

void ball_update(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);
	Eon_Coord cx, cy;
	int x, y;

	eon_square_coords_get((Eon_Square *)prv->shape, &cx, &cy, NULL, NULL);
	y = cy.value + prv->iy;
	x = cx.value;
	prv->error += prv->m;
	if (prv->error >= 1)
	{
		prv->error = 1 - prv->error;
		x = cx.value + prv->ix;
	}
	eon_rect_y_set(prv->shape, y);
	eon_rect_x_set(prv->shape, x);
}

void ball_bounce_x(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);

	printf("bouncing on x\n");
	prv->ix = -prv->ix;
	prv->error = 1;
}

void ball_bounce_y(Ball *b)
{
	Ball_Private *prv = PRIVATE(b);

	printf("bouncing on y\n");
	prv->iy = -prv->iy;
}

void ball_geometry_get(Ball *b, int *x, int *y, int *w, int *h)
{
	Ball_Private *prv = PRIVATE(b);
	Eon_Coord cx, cy, cw, ch;

	eon_square_coords_get((Eon_Square *)prv->shape, &cx, &cy, &cw, &ch);
	if (x) *x = cx.final;
	if (y) *y = cy.final;
	if (w) *w = cw.final;
	if (h) *h = ch.final;
}

void ball_pos_get(Ball *b, int x, int y)
{
	Ball_Private *prv = PRIVATE(b);

	eon_rect_y_set(prv->shape, y);
	eon_rect_x_set(prv->shape, x);
}

void ball_direction_get(Ball *b, int *tb, int *lr)
{
	Ball_Private *prv = PRIVATE(b);

	if (tb) *tb = prv->iy;
	if (lr) *lr = prv->ix;
}

void ball_direction_set(Ball *b, int tb, int lr)
{
	Ball_Private *prv = PRIVATE(b);

	prv->iy = tb;
	prv->ix = lr;
}


void ball_slope_set(Ball *b, float m)
{
	Ball_Private *prv = PRIVATE(b);

	if (m < 0) m = 0;
	if (m > 1) m = 1;
	prv->m = m;
	prv->error = 1;
}
