#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"
#include "reader.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component_Reader * equis_reader_new(Equis_Component *c)
{
	Equis_Component_Reader *r;

	r = calloc(1, sizeof(Equis_Component_Reader));
	r->from = c;
	
	equis_component_reader_new(c, r);
	return r;
}

EAPI void equis_reader_rewind(Equis_Component_Reader *r)
{
	r->points = r->from->points;
	r->cmds = r->from->cmds;
}

EAPI int equis_reader_vertex_get(Equis_Component_Reader *r, float *x, float *y)
{
#if 0
	equis_t *tmp_x, *tmp_y;

	tmp_x = p->x + (p->next_vertex / equis_c_elements);
	tmp_y = p->y + (p->next_vertex / equis_c_elements);
	cpu_path_vertex_get(tmp_x, tmp_y, x, y, 
		p->next_vertex % equis_c_elements);
	cpu_path_vertex_get(p->cpu_data, p->next_vertex, x, y);
	if (p->next_vertex++ > p->num_vertices)
	{
		p->next_vertex = 0;
	}
#endif
}
