#include <stdlib.h>
#include <string.h>

#include "Emage.h"

#define WIDTH 256
#define HEIGHT 256

int main(void)
{
	DATA32 *d1, *d2;
	RGBA_Surface *s1, *s2;
	RGBA_Draw_Context *dc;

	/* create a new surface */
	if (!emage_init()) return -1;

	d1 = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	d2 = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	memset(d1, 0xff, sizeof(DATA32) * WIDTH * HEIGHT);
	memset(d2, 0x34, sizeof(DATA32) * WIDTH/2 * HEIGHT/2);
	dc = emage_draw_context_new();
	s1 = emage_surface_new(d1, WIDTH, HEIGHT);
	s2 = emage_surface_new(d1, WIDTH, HEIGHT);

	/* objects test */
	evas_common_draw_context_set_color(dc, 128, 128, 128, 0);
	evas_common_draw_context_set_anti_alias(dc, 1);
	//evas_common_draw_context_set_multiplier(dc, 14, 26, 50, 30);
	emage_line_draw(s1, dc, 0, 0, WIDTH - 1, HEIGHT - 1);
	emage_rectangle_draw(s1, dc, 10, 10, 150, 150);
	evas_common_scale_rgba_in_to_out_clip_smooth(s2, s1, dc, 0, 0, 127, 127, 127, 127, 127, 127);
#if 0
	{
		int i;
		DATA32 *ptr;

		for (i = 0; i < WIDTH * HEIGHT; i++)
		{
			ptr = d1 + i;
			printf("value = %x\n", *ptr);
		}

	}
#endif
	png_save(s1, "/tmp/emage.png", 0);

	free(d1);
	free(d2);
	free(dc);
	free(s1);
	free(s2);

	emage_shutdown();
	return 0;
}
