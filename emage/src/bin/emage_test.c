#include <stdlib.h>
#include <string.h>

#include "Emage.h"

#define WIDTH 256
#define HEIGHT 256

int main(void)
{
	DATA32 *d;
	RGBA_Surface *s;
	RGBA_Draw_Context *dc;

	/* create a new surface */
	if (!emage_init()) return -1;

	d = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	memset(d, 0xff, sizeof(DATA32) * WIDTH * HEIGHT);
	dc = emage_draw_context_new();
	s = emage_surface_new(d, WIDTH, HEIGHT);

	/* objects test */
	evas_common_draw_context_set_color(dc, 128, 128, 128, 40);
	evas_common_draw_context_set_anti_alias(dc, 1);
	evas_common_draw_context_set_multiplier(dc, 14, 26, 50, 30);
	emage_line_draw(s, dc, 0, 0, WIDTH - 1, HEIGHT - 1);
	emage_rectangle_draw(s, dc, 10, 10, 150, 150);
#if 0
	{
		int i;
		DATA32 *ptr;

		for (i = 0; i < WIDTH * HEIGHT; i++)
		{
			ptr = d + i;
			printf("value = %x\n", *ptr);
		}

	}
#endif
	png_save(s, "/tmp/emage.png", 0);

	free(d);
	free(dc);
	free(s);

	emage_shutdown();
	return 0;
}
