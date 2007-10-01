#include "Enginy.h"
#include "enginy_private.h"
#include "component.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static const char _name[] = "bop";

typedef struct _Enginy_Bop
{
	Enginy_Component 	*c;
} Enginy_Bop;

static void enginy_bop_generate(void *data, int *num)
{
	Enginy_Bop *d = data;
	float x, y;
	int cmd, i = 0;
	
}

static void enginy_bop_free(void *data)
{
	Enginy_Bop *d = data;

	free(d);
}

static void enginy_bop_init(Enginy_Component *c)
{
	Enginy_Bop *d;

	d = calloc(1, sizeof(Enginy_Bop));

	d->c = c;
	c->data = d;
	c->name = _name;
	c->type = EQUIS_COMPONENT_IO;
	c->generate = enginy_bop_generate;
	c->free = enginy_bop_free;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enginy_Component * enginy_bop_new(void)
{
	Enginy_Component *c;

	c = enginy_component_new();
	enginy_bop_init(c);
	return c;
}
