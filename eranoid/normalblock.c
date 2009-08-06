#include "eranoid.h"


static Ekeko_Type *normalblock_type_get(void);
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((NormalBlock_Private *)((NormalBlock *)(d))->prv)

struct _NormalBlock_Private
{
	// the color
	Eon_Rect *r;
};

static void hit(Obstacle *o, Ball *ball, Eina_Bool top)
{
	Block *b = (Block *)o;

	printf("BALL hit on the normalblock\n");
	if (top)
		ball_bounce_y(ball);
	else
		ball_bounce_x(ball);

	ekeko_object_delete((Ekeko_Object *)o);
	/* TODO remove the block from the grid */
	/* TODO send a signal to the animation system?? :) */
}


static void _ctor(void *instance)
{
	NormalBlock *b;
	NormalBlock_Private *prv;

	b = (NormalBlock *)instance;
	b->prv = prv = ekeko_type_instance_private_get(normalblock_type_get(), instance);
	b->base.base.hit = hit;
}

static void _dtor(void *instance)
{
	NormalBlock *b = instance;
	NormalBlock_Private *prv = PRIVATE(b);

	ekeko_object_delete((Ekeko_Object *)prv->r);
}

static Ekeko_Type *normalblock_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_NORMALBLOCK, sizeof(NormalBlock),
				sizeof(NormalBlock_Private), block_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
NormalBlock *normalblock_new(Eon_Canvas *c, int row, int col)
{
	NormalBlock *b;
	NormalBlock_Private *prv;
	Eon_Rect *r;

	b = ekeko_type_instance_new(normalblock_type_get());
	block_row_set(b, row);
	block_col_set(b, col);

	r = eon_rect_new(c);
	eon_rect_x_set(r, col * BLOCKW);
	eon_rect_y_set(r, row * BLOCKH);
	eon_rect_w_set(r, BLOCKW);
	eon_rect_h_set(r, BLOCKH);
	eon_rect_color_set(r, 0xff000000);
	eon_rect_show(r);

	prv = PRIVATE(b);
	prv->r = r;

	return b;
}
