#include "Equis.h"
#include "equis_private.h"

void equis_path_scale(Equis_Path *s, Equis_Path *d, float sx, float sy)
{
	/* handle all possible constraints */
	cpu_path_scale(s->x, s->y, d->x, d->y, sx, sy, s->num_vertices);
}
