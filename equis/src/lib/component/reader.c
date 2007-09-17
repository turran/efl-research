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
	r->pos = 0;
}

EAPI int equis_reader_vertex_get(Equis_Component_Reader *r, float *x, float *y)
{
	*x = r->points->x;
	*y = r->points->y;
	r->points++;
	r->cmds++;
	r->pos;
}
