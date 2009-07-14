#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>

#include "Eon.h"

/*
 * The player is an application that parses external files with eon scenes
 *
 * TODO make eon type system to be defined at eon_init() to avoid strcmp => eon_foo
 * attributes of type VALUE are relative to another attribute, how to handle it?
 *
 */
char *engine = "sdl";
char *image_dir = "./";

void help(void)
{
	printf("eon_player [OPTIONS] FILE\n");
	printf("-h This help\n");
	printf("-e Engine\n");
	printf("-W Width\n");
	printf("-H Height\n");
	printf("-I Image directory\n");
	printf("FILE Eon XML file\n");
}

int main(int argc, char **argv)
{
	Eon_Document *doc;
	Eon_External *ext;
	Eon_Canvas *canvas;
	int w = 320;
	int h = 240;

	char *short_options = "I:e:hW:H:";
	struct option long_options[] = {
		{"image", 1, 0, 'I'},
		{"engine", 1, 0, 'e'},
		{"help", 0, 0, 'h'},
		{"width", 1, 0, 'W'},
		{"height", 1, 0, 'H'},
		{0, 0, 0, 0}
	};
	int option;
	char c;
	char *file = argv[argc - 1];

	while ((c = getopt_long(argc, argv, short_options, long_options,
			&option)) != -1)
	{
		switch (c)
		{
			case 'h':
			help();
			return 0;

			case 'W':
			w = atoi(optarg);
			break;

			case 'I':
			image_dir = strdup(optarg);
			break;

			case 'H':
			h = atoi(optarg);
			break;

			default:
			help();
			return 0;
			break;
		}
	}
	/* create the context */
	eon_init();
	doc = eon_document_new(engine, w, h);
	/* create the canvas */
	canvas = eon_document_canvas_get(doc);
	/* create the external object */
	ext = eon_external_new(canvas);
	eon_external_file_set(ext, file);
	eon_loop();
	eon_shutdown();

	return 0;
}
