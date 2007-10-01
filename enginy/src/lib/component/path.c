#include "Enginy.h"
#include "enginy_private.h"

static const char type[] = "path";

typedef struct _Enginy_Path
{

} Enginy_Path;

static Enginy_Path * enginy_path_create(void)
{
	Enginy_Path *p;

	p = calloc(1, sizeof(Enginy_Path));
	return p;
}

static void enginy_path_init(Enginy_Generator *g)
{
	g->data = enginy_path_create();
}

EAPI Enginy_Generator * enginy_path_new(void)
{
	Enginy_Generator *g;

	g = enginy_generator_new();
	enginy_path_init(g);
	return g;
}

EAPI void enginy_path_move_to(Enginy_Generator *p, int x, int y)
{

}

EAPI void enginy_path_line_to(Enginy_Generator *p, int x, int y)
{

}

EAPI void enginy_path_close(Enginy_Generator *p)
{

}
