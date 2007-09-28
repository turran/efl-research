#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"
#include "reader.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* called whenever the pointer to the path has changed */
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
/**
 * To be documented
 * FIXME: To be fixed
 */
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

/**
 * To be documented
 * FIXME: To be fixed
 */
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

/**
 * To be documented
 * FIXME: To be fixed
 */
/* should create maximum num vertices */
int equis_component_generate(Equis_Component *c, int *num)
{
	/* if the component is of type input-ouput and we dont
	 * have a source of vertices, just return */
	if ((c->type == EQUIS_COMPONENT_IO) && (!c->src))
	{
		return 0;
	}
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
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equis_component_delete(Equis_Component *c)
{
	if (c->free)
		c->free(c->data);
	/* TODO what about all the readers attached */
	free(c);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int equis_component_source_set(Equis_Component *c, Equis_Component *src)
{
	assert(c);
	assert(src);

	/* output component only */
	if (c->type == EQUIS_COMPONENT_O)
		return -EQUIS_ERROR_INVAL;
	/* same source, do nothing */
	if ((c->src) && (c->src->from == src))
		return EQUIS_ERROR_NONE;
	if (!c->src)
		c->src = equis_reader_new(src);
	/* notify the change */
	equis_component_notify(c);

	return EQUIS_ERROR_NONE;
}
