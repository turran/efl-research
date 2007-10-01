#include "Enesim.h"
#include "enesim_private.h"
#include "component.h"

static const char type[] = "path";

typedef struct _Enesim_Path
{

} Enesim_Path;

static Enesim_Path * enesim_path_create(void)
{
	Enesim_Path *p;

	p = calloc(1, sizeof(Enesim_Path));
	return p;
}

static void enesim_path_init(Enesim_Generator *g)
{
	g->data = enesim_path_create();
}

EAPI Enesim_Generator * enesim_path_new(void)
{
	Enesim_Generator *g;

	g = enesim_generator_new();
	enesim_path_init(g);
	return g;
}

EAPI void enesim_path_move_to(Enesim_Generator *p, int x, int y)
{

}

EAPI void enesim_path_line_to(Enesim_Generator *p, int x, int y)
{

}

EAPI void enesim_path_close(Enesim_Generator *p)
{

}
