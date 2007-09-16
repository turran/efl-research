#include "Equis.h"
#include "equis_private.h"
#include "Edata.h"
#include "component.h"

static const char _type[] = "source_csv";

typedef struct _Equis_Source_Csv
{
	char *file;
} Equis_Source_Csv;

static void equis_source_csv_init(Equis_Component *c, const char *f)
{
	Equis_Source_Csv *d;

	d = calloc(1, sizeof(Equis_Source_Csv));
	d->file = strdup(f);

	c->data = d;
	c->type = _type;
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

}
