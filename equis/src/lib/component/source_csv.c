#include <stdio.h>

#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

/* TODO
 * - check for existance of the file
 *
 */

static const char _type[] = "source_csv";

typedef struct _Equis_Source_Csv
{
	Equis_Component *c;
	char 		*name;
	FILE 		*f;
} Equis_Source_Csv;


static void equis_source_csv_open(Equis_Source_Csv *d, const char *name)
{
	if (!name) return;
	d->name = strdup(name);
	d->f = fopen(name, "r");
	/* if !f d->name = NULL */
}

static void equis_source_csv_close(Equis_Source_Csv *d)
{
	free(d->name);
	d->name = NULL;
	fclose(d->f);
}


static void equis_source_csv_generate(void *data, int *num)
{
	Equis_Source_Csv *d = data;
	int i = 0;
	float x, y;

	
	/* first vertex we generate should have a MOVE command */
	if (!d->c->path->num_vertices)
	{
		if (fscanf(d->f, "%f %f\n", &x, &y) != EOF)
		{
			equis_path_vertex_add(d->c->path, EQUIS_CMD_MOVE_TO, x, y);
			i++;
		}
	}
	/* next vertices should be a LINETO */
	while ((i < *num) && ((fscanf(d->f, "%f %f\n", &x, &y) != EOF)))
	{
		equis_path_vertex_add(d->c->path, EQUIS_CMD_LINE_TO, x, y);
		i++;
	}
	/* last vertex we generate must be an END command */
	if (i < *num)
	{
		equis_path_vertex_add(d->c->path, EQUIS_CMD_END, 0, 0);
	}
	/* set the correct number of vertices calculated */
	*num = i;
}

static void equis_source_csv_free(void *data)
{
	Equis_Source_Csv *d = data;
}

static void equis_source_csv_init(Equis_Component *c)
{
	Equis_Source_Csv *d;

	d = calloc(1, sizeof(Equis_Source_Csv));

	d->c = c;
	c->data = d;
	c->type = _type;
	c->generate = equis_source_csv_generate;
	c->free = equis_source_csv_free;
}


/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_source_csv_new(void)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_source_csv_init(c);
	return c;
}

EAPI int equis_source_csv_file_set(Equis_Component *c, const char *path)
{
	Equis_Source_Csv *d;

	d = c->data;
	if (d->name)
		equis_source_csv_close(d);
	equis_source_csv_open(d, path);
	/* we have changed the file, propagate it */
	equis_component_notify(c);
	return 1;
}
