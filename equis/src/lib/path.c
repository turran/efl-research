#include "Equis.h"
#include "equis_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void equis_path_transform(Equis_Path *s, Equis_Path *d, float *tm)
{
	/* handle all possible cases */
	/* check the dst path space */
	/* empty matrix */
	/* identity matrix */
	if (MATRIX_IS_SCALE(tm))
	{
		cpu_path_scale(s->cpu_data, d->cpu_data, s->num_vertices,
			tm[0], tm[3]);
		return;
	}
	if (MATRIX_IS_SHEAR_X(tm))
	{
		//cpu_path_shear(s->cpu_data, d->cpu_data, s->num_vertices,
		//	tm[1]);
		return;
	}
	if (MATRIX_IS_SHEAR_Y(tm))
	{
		//cpu_path_shear(s->cpu_data, d->cpu_data, s->num_vertices,
		//	tm[2]);
		return;
	}
	/* general case */
}

EAPI Equis_Path * equis_path_new(int num_vertices)
{
	Equis_Path *p;

	p = calloc(1, sizeof(Equis_Path));
	p->num_vertices = num_vertices;
	cpu_path_new(&p->cpu_data, num_vertices);
#if 0
	if (num_vertices)
	{
		p->num_allocated = cpu_path_alloc(&p->x, &p->y, num_vertices);
		p->num_vertices = num_vertices;
	}
#endif
	return p;
}

EAPI void equis_path_delete(Equis_Path *p)
{
#if 0
	free(p->x);
	free(p->y);
#endif
	free(p);
}

/* for now */
EAPI void equis_path_vertex_add(Equis_Path *p, float x, float y)
{
#if 0
	equis_t *tmp_x, *tmp_y;
	
	tmp_x = p->x + (p->next_vertex / equis_c_elements);
	tmp_y = p->y + (p->next_vertex / equis_c_elements);
	cpu_path_vertex_add(tmp_x, tmp_y, x, y, 
		p->next_vertex % equis_c_elements);
#endif
	cpu_path_vertex_add(p->cpu_data, p->next_vertex, x, y);
	if (p->next_vertex++ > p->num_vertices)
	{
		/* TODO alloc more space */
		p->next_vertex = 0;
	}
}

EAPI void equis_path_vertex_get(Equis_Path *p, float *x, float *y)
{
#if 0
	equis_t *tmp_x, *tmp_y;

	tmp_x = p->x + (p->next_vertex / equis_c_elements);
	tmp_y = p->y + (p->next_vertex / equis_c_elements);
	cpu_path_vertex_get(tmp_x, tmp_y, x, y, 
		p->next_vertex % equis_c_elements);
#endif
	cpu_path_vertex_get(p->cpu_data, p->next_vertex, x, y);
	if (p->next_vertex++ > p->num_vertices)
	{
		p->next_vertex = 0;
	}
}

EAPI void equis_path_rewind(Equis_Path *p)
{
	p->next_vertex = 0;
}
