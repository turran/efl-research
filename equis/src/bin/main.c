#include "Equis.h"
#include "equis_private.h"

int main(void)
{
	Equis_Path p1, p2;

	p1.x = malloc(sizeof(equis_t) * 1000);
	p1.y = malloc(sizeof(equis_t) * 1000);
	p2.x = malloc(sizeof(equis_t) * 1000);
	p2.y = malloc(sizeof(equis_t) * 1000);

	equis_path_scale(&p1, &p2, 2.5, 3.0);

	free(p1.x);
	free(p1.y);
	free(p2.x);
	free(p2.y);
	return 0;
}
