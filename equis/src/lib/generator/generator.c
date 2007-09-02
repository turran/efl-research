#include "Equis.h"
#include "equis_private.h"

Equis_Generator * equis_generator_new(void)
{
	Equis_Generator *g;

	g = calloc(1, sizeof(Equis_Generator));
	return g;
}

void
equis_generator_get_vertex(Equis_Generator *g, int *x, int *y, Equis_Vertex_Command cmd)
{

}
