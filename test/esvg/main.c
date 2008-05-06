#include <stdlib.h>
#include <stdio.h>

#include "Eina.h"
#include "ESVG.h"


static void help(void)
{
	printf("Usage esvg_test <FILE> <ENGINE>\n");
	printf("Open FILE and render it using ENGINE's backend.\n\n");
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
	ESVG *esvg;
	
	if (argc < 3)
	{
		help();
		return -1;
	}
	/* call the backend init */
	/* forever */
	/* call the backend loop */
	/* call the test loop */
	/* call the backend shutdown */
	
	esvg = esvg_document_load(argv[1], ESVG_ENGINE_CAIRO, NULL);
	return 1;
}
