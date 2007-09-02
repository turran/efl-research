#include "Equis.h"
#include "equis_private.h"

static const char type[] = "path";

typedef struct _Equis_Path
{

} Equis_Path;

static Equis_Path * equis_path_create(void)
{
	Equis_Path *p;

	p = calloc(1, sizeof(Equis_Path));
	return p;
}

static void equis_path_init(Equis_Generator *g)
{
	g->data = equis_path_create();
}

EAPI Equis_Generator * equis_path_new(void)
{
	Equis_Generator *g;

	g = equis_generator_new();
	equis_path_init(g);
	return g;
}

EAPI void equis_path_move_to(Equis_Generator *p, int x, int y)
{

}

EAPI void equis_path_line_to(Equis_Generator *p, int x, int y)
{

}

EAPI void equis_path_close(Equis_Generator *p)
{

}
