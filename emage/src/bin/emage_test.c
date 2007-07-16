#include "Emage.h"

#define WIDTH 256
#define HEIGHT 256

int main(void)
{
	DATA32 *d;
	RGBA_Surface *s;
	RGBA_Draw_Context *dc;

	/* create a new surface */
	d = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	dc = emage_draw_context_new();
	s = emage_surface_new(d, WIDTH, HEIGHT);

	free(d);
	free(dc);
	free(s);
}
