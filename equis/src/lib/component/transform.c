#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

static const char _type[] = "transform";

typedef struct _Equis_Transform
{
	Equis_Component *dst;
} Equis_Transform;


static void equis_transform_init(Equis_Component *c, const char *f)
{
	Equis_Transform *d;

	d = calloc(1, sizeof(Equis_Transform));

	c->data = d;
	c->type = _type;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_transform_new(float *matrix)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_transform_init(c, matrix);
	return c;
}

EAPI void equis_transform_matrix_set(Equis_Component *c, float matrix)
{
	Equis_Transform *d;

	d = c->data;
}

