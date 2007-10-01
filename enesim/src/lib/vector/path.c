#include "Enginy.h"
#include "enginy_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _a_alloc(Enginy_Path *p, int num)
{
	p->points = realloc(p->points, sizeof(Enginy_Point) * num);
	p->cmds = realloc(p->cmds, sizeof(char) * num);
	/* FIXME only add the offset and call the alloc_cb in case the pointer
	 * has changed */
	/* in case the realloc returns a different pointer */
	p->point_curr = p->points + p->num_vertices;
	p->cmd_curr = p->cmds + p->num_vertices;
	/* every time we alloc new data call the alloc_cb */
	p->alloc_cb(p->data);
}

static void _a_free(Enginy_Path *p)
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
Enginy_Path * enginy_path_new(void *data, int vertices_ref)
{
	Enginy_Path *p;

	p = calloc(1, sizeof(Enginy_Path));
	p->data = data;
	p->a = edata_array_new(p, EDATA_ARRAY_ALLOC(_a_alloc),
		EDATA_ARRAY_FREE(_a_free));
	return p;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void * enginy_path_delete(Enginy_Path *p)
{
	free(p->points);
	free(p->cmds);
	
	return p->data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void enginy_path_vertex_add(Enginy_Path *p, float x, float y, char cmd)
{
	edata_array_element_new(p->a);
	p->point_curr->x = x;
	p->point_curr->y = y;
	*p->cmd_curr = cmd;
	p->num_vertices++;
	p->point_curr++;
	p->cmd_curr++;
}