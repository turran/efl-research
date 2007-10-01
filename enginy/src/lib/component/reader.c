#include "Enginy.h"
#include "enginy_private.h"
#include "Edata.h"
#include "component.h"
#include "reader.h"

/**
 * TODO
 * flat a pipeline
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static inline _rewind(Enginy_Component_Reader *r)
{
	r->points = r->from->path->points;
	r->cmds = r->from->path->cmds;
	r->pos = 0;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 *
 */
void enginy_reader_notify(Enginy_Component_Reader *r)
{
	/* FIXME update the pointers as the component might have had to
	 * realloc its data */
	if (r->to && !r->to->has_changed)
	{
		enginy_component_notify(r->to);
	}
}

/**
 * called when the path of a component has realloced its data 
 */
void enginy_reader_reference_update(Enginy_Component_Reader *r)
{
	r->points = r->from->path->points;
	r->cmds = r->from->path->cmds;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enginy_Component_Reader * enginy_reader_new(Enginy_Component *c)
{
	Enginy_Component_Reader *r;

	r = calloc(1, sizeof(Enginy_Component_Reader));
	r->from = c;
	_rewind(r);
	/* append the reader to the list of readers,
	 * if we add a reader to a component that has changed
	 * propagate that change to the other end of the reader
	 */
	edata_list_append(c->readers, r);
	return r;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enginy_reader_rewind(Enginy_Component_Reader *r)
{
	_rewind(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int enginy_reader_vertex_get(Enginy_Component_Reader *r, float *x, float *y)
{
	int ret;
	int num = 1;

	assert(r->from);
	if (r->pos >= r->from->path->num_vertices)
	{
		if (!enginy_component_generate(r->from, &num))
			return EQUIS_CMD_END;
		if (num < 1)
			return EQUIS_CMD_END;
	}
	*x = r->points->x;
	*y = r->points->y;
	ret = *r->cmds;
	r->points++;
	r->pos++;
	r->cmds++;
	return ret;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int enginy_reader_vertices_get(Enginy_Component_Reader *r, float *x, float *y, int *cmds)
{


}
