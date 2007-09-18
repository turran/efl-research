#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _alloc_cb(void *data)
{
	Equis_Component_Reader *r;
	Equis_Component *c = data;
	
	edata_list_first_goto(c->readers);
	while ((r = edata_list_next(c->readers)))
	{
		equis_reader_reference_update(r);
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Equis_Component * equis_component_new(void)
{
	Equis_Component *c;

	c = calloc(1, sizeof(Equis_Component));
	c->readers = edata_list_new();
	c->path = equis_path_new(c, 0);
	/* ABSTRACT THIS */
	c->path->alloc_cb = _alloc_cb;
	return c;
}

void equis_component_notify(Equis_Component *c)
{
	Equis_Component_Reader *r;
	
	if (c->has_changed) return;
	
	c->has_changed = 1;
	edata_list_first_goto(c->readers);
	while ((r = edata_list_next(c->readers)))
	{
		equis_reader_notify(r);
	}
}

/* should create maximum num vertices */
int equis_component_generate(Equis_Component *c, int *num)
{
	if (c->generate)
	{
		c->generate(c->data, num);
		return 1;
	}
	return 0;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void equis_component_delete(Equis_Component *c)
{
	if (c->free)
		c->free(c->data);
	/* TODO what about all the readers attached */
	free(c);
}
