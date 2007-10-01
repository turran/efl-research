#include "Enginy.h"
#include "enginy_private.h"
#include "Edata.h"
#include "component.h"
#include "reader.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* called whenever the pointer to the path has changed */
static void _alloc_cb(void *data)
{
	Enginy_Component_Reader *r;
	Enginy_Component *c = data;
	
	edata_list_first_goto(c->readers);
	while ((r = edata_list_next(c->readers)))
	{
		enginy_reader_reference_update(r);
	}
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Enginy_Component * enginy_component_new(void)
{
	Enginy_Component *c;

	c = calloc(1, sizeof(Enginy_Component));
	c->readers = edata_list_new();
	c->path = enginy_path_new(c, 0);
	/* ABSTRACT THIS */
	c->path->alloc_cb = _alloc_cb;
	return c;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void enginy_component_notify(Enginy_Component *c)
{
	Enginy_Component_Reader *r;
	
	if (c->has_changed) return;
	
	c->has_changed = 1;
	edata_list_first_goto(c->readers);
	while ((r = edata_list_next(c->readers)))
	{
		enginy_reader_notify(r);
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
/* should create maximum num vertices */
int enginy_component_generate(Enginy_Component *c, int *num)
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
EAPI void enginy_component_delete(Enginy_Component *c)
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
EAPI int enginy_component_source_set(Enginy_Component *c, Enginy_Component *src)
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
		c->src = enginy_reader_new(src);
	/* notify the change */
	enginy_component_notify(c);

	return EQUIS_ERROR_NONE;
}
