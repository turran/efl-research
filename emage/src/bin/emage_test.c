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
	memset(d, 0xff, sizeof(DATA32) * WIDTH * HEIGHT);
	dc = emage_draw_context_new();
	s = emage_surface_new(d, WIDTH, HEIGHT);

	/* objects test */
	emage_line_draw(s, dc, 0, 0, WIDTH - 1, HEIGHT - 1);
	
	png_save(s, "/tmp/emage.png", 0);

	free(d);
	free(dc);
	free(s);
}
