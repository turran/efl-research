#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

static const char _name[] = "transform";

typedef struct _Equis_Transform
{
	Equis_Component_Reader *src;
} Equis_Transform;


static void equis_transform_init(Equis_Component *c)
{
	Equis_Transform *d;

	d = calloc(1, sizeof(Equis_Transform));

	c->data = d;
	c->name = _name;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_transform_new(void)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_transform_init(c);
	return c;
}

EAPI void equis_transform_matrix_set(Equis_Component *c, float matrix)
{
	Equis_Transform *d;

	d = c->data;
}
