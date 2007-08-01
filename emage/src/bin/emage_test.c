#include "emage_test.h"

#define WIDTH 256
#define HEIGHT 512 

int main(void)
{
	DATA32 *d1, *d2;
	Emage_Data_ARGB8888 *data;
	
	Emage_Surface *s1, *s2;
	Emage_Draw_Context *dc;

	/* create a new surface */
	if (!emage_init()) return -1;

	data = calloc(1, sizeof(Emage_Data_ARGB8888));
	d1 = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	data->data = d1;
	//d2 = calloc(1, sizeof(DATA32) * (WIDTH/2) * (HEIGHT/2));
	memset(d1, 0xff, sizeof(DATA32) * WIDTH * HEIGHT);
	//memset(d2, 0xf0, sizeof(DATA32) * (WIDTH/2) * (HEIGHT/2));
	dc = emage_draw_context_new();
	s1 = emage_surface_new(data, EMAGE_DATA_ARGB8888, WIDTH, HEIGHT);
	//s2 = emage_surface_new(d2, EMAGE_DATA_ARGB8888, WIDTH/2, HEIGHT/2);

	/* common */
	emage_draw_context_set_color(dc, 128, 128, 0, 255);
	emage_draw_context_set_anti_alias(dc, 1);
	//emage_draw_context_set_multiplier(dc, 14, 26, 50, 30);
	/* objects test */
	emage_draw_context_set_render_op(dc, 0);
	object_test(s1, dc);
	//emage_draw_context_set_color(dc, 218, 0, 0, 255);
	//object_test(s2, dc);
	
	/* blt/blend */
	//evas_common_scale_rgba_in_to_out_clip_smooth(s2, s1, dc, 0, 0, 127, 127, 0, 0, 255, 255);
	#if 0
	{
		DATA32 *p;
		int y = 25;
		int x = 25;


		p = d1 + (y * WIDTH) + x;
   		printf("ptr at %p (%p) offset = %d\n", p, d1, (y * WIDTH) + x);
		*p = 0x00000000;
		//*(p + 1) = 0x00000000;

	}
	#endif
	
	png_save(s1, "/tmp/emage1.png", 0);
	png_save(s2, "/tmp/emage2.png", 0);

	free(d1);
	free(d2);
	free(dc);
	free(s1);
	free(s2);

	emage_shutdown();
	return 0;
}
