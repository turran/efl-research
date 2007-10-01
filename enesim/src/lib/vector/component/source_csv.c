#include <stdio.h>

#include "Enginy.h"
#include "enginy_private.h"
#include "component.h"

/* TODO
 * - check for existance of the file
 *
 */

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static const char _name[] = "source_csv";

typedef struct _Enginy_Source_Csv
{
	Enginy_Component *c;
	char 		*name;
	FILE 		*f;
} Enginy_Source_Csv;


static int enginy_source_csv_open(Enginy_Source_Csv *d, const char *name)
{
	if (!name) return;
	
	d->name = strdup(name);
	d->f = fopen(name, "r");
	/* if !f d->name = NULL */
	return 1;
}

static void enginy_source_csv_close(Enginy_Source_Csv *d)
{
	if (d->name)
	{
		free(d->name);
		d->name = NULL;
	}
	if (d->f)
	{
		fclose(d->f);
	}
}

static void enginy_source_csv_generate(void *data, int *num)
{
	Enginy_Source_Csv *d = data;
	int i = 0;
	float x, y;

	
	/* first vertex we generate should have a MOVE command */
	if (!d->c->path->num_vertices)
	{
		if (fscanf(d->f, "%f %f\n", &x, &y) != EOF)
		{
			enginy_path_vertex_add(d->c->path, x, y, EQUIS_CMD_MOVE_TO);
			i++;
		}
	}
	/* next vertices should be a LINETO */
	while ((i < *num) && (fscanf(d->f, "%f %f\n", &x, &y) != EOF))
	{
		enginy_path_vertex_add(d->c->path, x, y, EQUIS_CMD_LINE_TO);
		i++;
	}
	/* last vertex we generate must be an END command */
	if (i < *num)
	{
		enginy_path_vertex_add(d->c->path, 0, 0, EQUIS_CMD_END);
	}
	/* set the correct number of vertices calculated */
	*num = i;
}

static void enginy_source_csv_free(void *data)
{
	Enginy_Source_Csv *d = data;

	enginy_source_csv_close(d);
	free(d);
}

static void enginy_source_csv_init(Enginy_Component *c)
{
	Enginy_Source_Csv *d;

	d = calloc(1, sizeof(Enginy_Source_Csv));

	d->c = c;
	c->data = d;
	c->name = _name;
	c->generate = enginy_source_csv_generate;
	c->free = enginy_source_csv_free;
}


/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enginy_Component * enginy_source_csv_new(void)
{
	Enginy_Component *c;

	c = enginy_component_new();
	enginy_source_csv_init(c);
	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int enginy_source_csv_file_set(Enginy_Component *c, const char *path)
{
	Enginy_Source_Csv *d;

	assert(c);
	assert(c->data);

	d = c->data;
	enginy_source_csv_close(d);
	if (!enginy_source_csv_open(d, path))
		return EQUIS_ERROR_INVAL;
	/* we have changed the file, propagate it */
	enginy_component_notify(c);
	return EQUIS_ERROR_NONE;
}
