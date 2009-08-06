#include "eranoid.h"

#define PRIVATE(d) ((Block_Private *)((Block *)(d))->prv)
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Block_Private
{
	int row;
	int col;
};

static void _ctor(void *instance)
{
	Block *b;
	Block_Private *prv;

	b = (Block *)instance;
	b->prv = prv = ekeko_type_instance_private_get(block_type_get(), instance);
}

static void _dtor(void *instance)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Type *block_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_BLOCK, sizeof(Block),
				sizeof(Block_Private), obstacle_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

void block_position_get(Block *b, int *row, int *col)
{
	Block_Private *prv = PRIVATE(b);

	if (row) *row = prv->row;
	if (col) *col = prv->col;
}

void block_row_set(Block *b, int row)
{
	Block_Private *prv = PRIVATE(b);

	prv->row = row;
}

void block_col_set(Block *b, int col)
{
	Block_Private *prv = PRIVATE(b);

	prv->col = col;
}
