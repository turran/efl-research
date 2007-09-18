#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"
#include "reader.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static inline _rewind(Equis_Component_Reader *r)
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
void equis_reader_notify(Equis_Component_Reader *r)
{
	/* FIXME update the pointers as the component might have had to
	 * realloc its data */
	printf("notified\n");
	if (r->to && !r->to->has_changed)
	{
		equis_component_notify(r->to);
	}
}

/**
 * called when the path of a component has realloced its data 
 */
void equis_reader_reference_update(Equis_Component_Reader *r)
{
	r->points = r->from->path->points;
	r->cmds = r->from->path->cmds;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component_Reader * equis_reader_new(Equis_Component *c)
{
	Equis_Component_Reader *r;

	r = calloc(1, sizeof(Equis_Component_Reader));
	r->from = c;
	_rewind(r);
	printf("%p\n", r->points);
	/* append the reader to the list of readers,
	 * if we add a reader to a component that has changed
	 * propagate that change to the other end of the reader
	 */
	edata_list_append(c->readers, r);
	return r;
}

EAPI void equis_reader_rewind(Equis_Component_Reader *r)
{
	_rewind(r);
}

EAPI int equis_reader_vertex_get(Equis_Component_Reader *r, float *x, float *y)
{
	int ret;
	int num = 1;

	assert(r->from);
	if (r->pos >= r->from->path->num_vertices)
	{
		if (!equis_component_generate(r->from, &num))
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
