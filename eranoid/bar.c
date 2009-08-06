#include <stdio.h>
#include "eranoid.h"

static Ekeko_Type *bar_type_get(void);
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(b) ((Bar_Private *)((Bar *)(b))->prv)

struct _Bar_Private
{
	Eon_Rect *shape;
};

static void hit(Obstacle *o, Ball *ball, Eina_Bool top)
{
	printf("Bar hitted\n");
	ball_bounce_y(ball);
}

static void _ctor(void *instance)
{
	Bar *b;
	Bar_Private *prv;

	b = (Bar *)instance;
	b->prv = prv = ekeko_type_instance_private_get(bar_type_get(), instance);
	b->base.hit = hit;
}

static void _dtor(void *polygon)
{

}

static Ekeko_Type *bar_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_BAR, sizeof(Bar),
				sizeof(Bar_Private), obstacle_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

Bar * bar_new(Eon_Canvas *c)
{
	Bar *b;
	Bar_Private *prv;
	Eon_Rect *s;

	b = ekeko_type_instance_new(bar_type_get());
	prv = PRIVATE(b);

	s = eon_rect_new(c);

	eon_rect_x_set(s, 320/2 - 20);
	eon_rect_y_set(s, 240 - 5);
	eon_rect_w_set(s, 40);
	eon_rect_h_set(s, 10);
	eon_rect_color_set(s, 0xff0000ff);
	eon_rect_show(s);

	prv->shape = s;
	return b;
}

void bar_geometry_get(Bar *b, int *x, int *y, int *w, int *h)
{
	Bar_Private *prv = PRIVATE(b);;
	Eon_Coord cx, cy, cw, ch;

	eon_square_coords_get((Eon_Square *)prv->shape, &cx, &cy, &cw, &ch);
	if (x) *x = cx.final;
	if (y) *y = cy.final;
	if (w) *w = cw.final;
	if (h) *h = ch.final;
}
