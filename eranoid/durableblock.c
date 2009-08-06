#include "eranoid.h"


static Ekeko_Type *durableblock_type_get(void);
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((DurableBlock_Private *)((DurableBlock *)(d))->prv)

struct _DurableBlock_Private
{
	Eon_Rect *r;
};

static void hit(Obstacle *o, Ball *ball, Eina_Bool top)
{
	Block *b = (Block *)o;

	printf("BALL hit on the durableblock\n");
	if (top)
		ball_bounce_y(ball);
	else
		ball_bounce_x(ball);

	/* TODO send a signal to the animation system?? :) */
}


static void _ctor(void *instance)
{
	DurableBlock *b;
	DurableBlock_Private *prv;

	b = (DurableBlock *)instance;
	b->prv = prv = ekeko_type_instance_private_get(durableblock_type_get(), instance);
	b->base.base.hit = hit;
}

static void _dtor(void *instance)
{
	DurableBlock *b = instance;
	DurableBlock_Private *prv = PRIVATE(b);

	ekeko_object_delete((Ekeko_Object *)prv->r);
}

static Ekeko_Type *durableblock_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_DURABLEBLOCK, sizeof(DurableBlock),
				sizeof(DurableBlock_Private), block_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
DurableBlock *durableblock_new(Eon_Canvas *c, int row, int col)
{
	DurableBlock *b;
	DurableBlock_Private *prv;
	Eon_Rect *r;

	b = ekeko_type_instance_new(durableblock_type_get());
	block_row_set(b, row);
	block_col_set(b, col);

	r = eon_rect_new(c);
	eon_rect_x_set(r, col * BLOCKW);
	eon_rect_y_set(r, row * BLOCKH);
	eon_rect_w_set(r, BLOCKW);
	eon_rect_h_set(r, BLOCKH);
	eon_rect_color_set(r, 0xff444444);
	eon_rect_show(r);

	prv = PRIVATE(b);
	prv->r = r;

	return b;
}
