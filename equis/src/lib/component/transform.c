#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static const char _name[] = "transform";

typedef struct _Equis_Transform
{
	Equis_Component 	*c;
	float 			m[4];
} Equis_Transform;

static void equis_transform_generate(void *data, int *num)
{
	Equis_Transform *d = data;
	float x, y;
	int cmd, i = 0;
	
	while ((i < *num) && 
		((cmd = equis_reader_vertex_get(d->c->src, &x, &y)) != EQUIS_CMD_END))
	{
		float x2, y2;

		x2 = (x * d->m[0]) + (y * d->m[1]);
		y2 = (x * d->m[2]) + (y * d->m[3]);
		equis_path_vertex_add(d->c->path, x2, y2, cmd);
		i++;
	}
	/* last vertex we generate must be an END command */
	if (i < *num)
	{
		equis_path_vertex_add(d->c->path, 0, 0, EQUIS_CMD_END);
	}
	*num = i;
}

static void equis_transform_free(void *data)
{
	Equis_Transform *d = data;

	free(d);
}

static void equis_transform_init(Equis_Component *c)
{
	Equis_Transform *d;

	d = calloc(1, sizeof(Equis_Transform));

	d->c = c;
	c->data = d;
	c->name = _name;
	c->type = EQUIS_COMPONENT_IO;
	c->generate = equis_transform_generate;
	c->free = equis_transform_free;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_transform_new(void)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_transform_init(c);
	return c;
}

EAPI void equis_transform_matrix_set(Equis_Component *c, float *matrix)
{
	Equis_Transform *d;

	d = c->data;
	d->m[0] = matrix[0];
	d->m[1] = matrix[1];
	d->m[2] = matrix[2];
	d->m[3] = matrix[3];
	equis_component_notify(c);
}
