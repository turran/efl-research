#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

static const char _type[] = "source_csv";

typedef struct _Equis_Source_Csv
{
	Equis_Component *c;
	char 		*file;
} Equis_Source_Csv;


static void equis_source_csv_open(Equis_Source_Csv *d, const char *f)
{
	if (!f) return;
	d->file = strdup(f);
}

static void equis_source_csv_close(Equis_Source_Csv *d)
{
	free(d->file);
}


static void equis_source_csv_generate(void *data, int *num)
{

}

static void equis_source_csv_delete(void *data)
{

}

static void equis_source_csv_init(Equis_Component *c, const char *f)
{
	Equis_Source_Csv *d;

	d = calloc(1, sizeof(Equis_Source_Csv));

	d->c = c;
	c->data = d;
	c->type = _type;
	equis_source_csv_open(d, f);
}


/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equis_Component * equis_source_csv_new(const char *f)
{
	Equis_Component *c;

	c = equis_component_new();
	equis_source_csv_init(c, f);
	return c;
}

EAPI void equis_source_csv_file_set(Equis_Component *c, const char *f)
{
	Equis_Source_Csv *d;

	d = c->data;
	if (d->file)
		equis_source_csv_close(c);
	equis_source_csv_open(c, f);
}
