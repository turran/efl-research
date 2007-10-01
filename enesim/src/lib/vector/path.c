#include "Enesim.h"
#include "enesim_private.h"
#include "path.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _a_alloc(Enesim_Path *p, int num)
{
	p->points = realloc(p->points, sizeof(Enesim_Point) * num);
	p->cmds = realloc(p->cmds, sizeof(char) * num);
	/* FIXME only add the offset and call the alloc_cb in case the pointer
	 * has changed */
	/* in case the realloc returns a different pointer */
	p->point_curr = p->points + p->num_vertices;
	p->cmd_curr = p->cmds + p->num_vertices;
	/* every time we alloc new data call the alloc_cb */
	p->alloc_cb(p->data);
}

static void _a_free(Enesim_Path *p)
{
	free(p->points);
	free(p->cmds);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Enesim_Path * enesim_path_new(void *data, int vertices_ref)
{
	Enesim_Path *p;

	p = calloc(1, sizeof(Enesim_Path));
	p->data = data;
	p->a = edata_array_new(p, EDATA_ARRAY_ALLOC(_a_alloc),
		EDATA_ARRAY_FREE(_a_free));
	return p;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void * enesim_path_delete(Enesim_Path *p)
{
	free(p->points);
	free(p->cmds);
	
	return p->data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void enesim_path_vertex_add(Enesim_Path *p, float x, float y, char cmd)
{
	edata_array_element_new(p->a);
	p->point_curr->x = x;
	p->point_curr->y = y;
	*p->cmd_curr = cmd;
	p->num_vertices++;
	p->point_curr++;
	p->cmd_curr++;
}
