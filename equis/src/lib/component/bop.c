#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static const char _name[] = "bop";

typedef struct _Equis_Bop
{
	Equis_Component 	*c;
} Equis_Bop;

static void equis_bop_generate(void *data, int *num)
{
	Equis_Bop *d = data;
	float x, y;
	int cmd, i = 0;
	
}

static void equis_bop_free(void *data)
{
	Equis_Bop *d = data;

	free(d);
}

static void equis_bop_init(Equis_Component *c)
{
	Equis_Bop *d;

	d = calloc(1, sizeof(Equis_Bop));

	d->c = c;
	c->data = d;
	c->name = _name;
	c->type = EQUIS_COMPONENT_IO;
	c->generate = equis_bop_generate;
	c->free = equis_bop_free;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_bop_new(void)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_bop_init(c);
	return c;
}
