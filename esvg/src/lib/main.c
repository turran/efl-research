#include "ESVG.h"
#include "esvg_private.h"

struct _ESVG
{
	
};

static ESVG * esvg_new(void)
{
	ESVG *svg;
	
	svg = calloc(1, sizeof(ESVG));
	return svg;
}

EAPI void esvg_file_save(const char *file)
{
	
}

EAPI ESVG* esvg_file_load(const char *file)
{
	ESVG *svg;
	
	svg = esvg_new();
	return svg;
}
