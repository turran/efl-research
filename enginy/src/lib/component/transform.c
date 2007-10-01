#include "Enginy.h"
#include "enginy_private.h"
#include "component.h"


/**
 *
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static const char _name[] = "transform";

typedef struct _Transform
{
	Enginy_Component 	*c;
	float 			m[4];
} Transform;

static void enginy_transform_generate(void *data, int *num)
{
	Transform *d = data;
	float x, y;
	int cmd, i = 0;
	
	while ((i < *num) && 
		((cmd = enginy_reader_vertex_get(d->c->src, &x, &y)) != EQUIS_CMD_END))
	{
		float x2, y2;

		x2 = (x * d->m[0]) + (y * d->m[1]);
		y2 = (x * d->m[2]) + (y * d->m[3]);
		enginy_path_vertex_add(d->c->path, x2, y2, cmd);
		i++;
	}
	/* last vertex we generate must be an END command */
	if (i < *num)
	{
		enginy_path_vertex_add(d->c->path, 0, 0, EQUIS_CMD_END);
	}
	*num = i;
}

static void enginy_transform_free(void *data)
{
	Transform *d = data;

	free(d);
}

static void enginy_transform_init(Enginy_Component *c)
{
	Transform *d;

	d = calloc(1, sizeof(Transform));

	d->c = c;
	c->data = d;
	c->name = _name;
	c->type = EQUIS_COMPONENT_IO;
	c->generate = enginy_transform_generate;
	c->free = enginy_transform_free;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enginy_Component * enginy_transform_new(void)
{
	Enginy_Component *c;

	c = enginy_component_new();
	enginy_transform_init(c);
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enginy_transform_matrix_set(Enginy_Component *c, float *matrix)
{
	Transform *d;

	d = c->data;
	d->m[0] = matrix[0];
	d->m[1] = matrix[1];
	d->m[2] = matrix[2];
	d->m[3] = matrix[3];
	enginy_component_notify(c);
}
