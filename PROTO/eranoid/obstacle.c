#include "eranoid.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Obstacle_Private
{

};

static void _ctor(void *instance)
{
	Obstacle *b;
	Obstacle_Private *prv;

	b = (Obstacle *)instance;
	b->prv = prv = ekeko_type_instance_private_get(obstacle_type_get(), instance);
}

static void _dtor(void *instance)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ekeko_Type *obstacle_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_BLOCK, sizeof(Obstacle),
				sizeof(Obstacle_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

void obstacle_hitted(Obstacle *o, Ball *b, Eina_Bool top)
{
	if (o->hit)
		o->hit(o, b, top);
}
