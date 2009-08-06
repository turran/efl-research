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
	Eon_Paint *p;
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
	Eon_Image *i;
	Eon_Coord coord;
	Ekeko_Value v;

	b = ekeko_type_instance_new(normalblock_type_get());
	block_row_set(b, row);
	block_col_set(b, col);

	r = eon_rect_new(c);
	eon_rect_x_set(r, col * BLOCKW);
	eon_rect_y_set(r, row * BLOCKH);
	eon_rect_w_set(r, BLOCKW);
	eon_rect_h_set(r, BLOCKH);
	eon_rect_color_set(r, 0xffee0000);
	eon_rect_show(r);

	i = eon_image_new();
	ekeko_object_child_append(c, i);
	eon_coord_set(&coord, 0, EON_COORD_RELATIVE);
	eon_value_coord_from(&v, &coord);
	ekeko_object_property_value_set(i, "x", &v);
	ekeko_object_property_value_set(i, "y", &v);
	eon_coord_set(&coord, 100, EON_COORD_RELATIVE);
	ekeko_object_property_value_set(i, "w", &v);
	ekeko_object_property_value_set(i, "h", &v);
#if 0
	/* FIXME implement this! */
	eon_paint_x_set(i, 0);
	eon_paint_y_set(i, 0);
	eon_paint_w_set(i, BLOCKW);
	eon_paint_h_set(i, BLOCKH);
#endif
	eon_image_file_set(i, "data/block.png");
	eon_shape_fill_paint_set(r, (Eon_Paint *)i);

	prv = PRIVATE(b);
	prv->r = r;

	return b;
}
