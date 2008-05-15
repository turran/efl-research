#include "esvg_test.h"

extern cairo_surface_t *src_cairo;

static void help(void)
{
	printf("Usage esvg_test <WIDTH> <HEIGHT> <FILE> <ENGINE>\n");
	printf("Open FILE and render it using ENGINE's backend.\n\n");
	printf("WIDTH: Width of the window\n");
	printf("HEIGHT: Height of the window\n");
	printf("ENGINES:\n");
	printf("\t sdl - SDL (Simple DirectMedia Layer) engine.\n");
}

static ESVG_Engine_Type parse_engine(const char *engine)
{
	if (!strcmp(engine, "sdl"))
	{
		
	}
}


int main(int argc, char **argv)
{
	ESVG_Svg *esvg;
	unsigned int w, h;
	unsigned int end = 0;
	
	if (argc < 5)
	{
		help();
		return -1;
	}
	w = strtol(argv[1], NULL, 0);
	h = strtol(argv[2], NULL, 0);
	/* call the backend init */
	if (!sdl_init(w, h))
		return -1;
	/* forever */
	esvg = esvg_document_load(argv[3], w, h, ESVG_ENGINE_CAIRO, src_cairo);
	while (!end)
	{
		/* call the backend loop */
		end = sdl_loop(esvg);
		esvg_svg_redraw_force(esvg);
	}
	/* call the backend shutdown */
	sdl_shutdown();
	return 1;
}
