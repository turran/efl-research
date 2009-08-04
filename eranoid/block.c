#include "eranoid.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Block_Private
{

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
