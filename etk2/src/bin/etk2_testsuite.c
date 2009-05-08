#include <stdio.h>
#include <string.h>

#include "Etk2.h"
#include "EXML.h"

/*
 * The testsuite is an application that parses xml files with etk scenes
 * TODO make etk2 type system to be defined at etk_init() to avoid strcmp => etk_foo
 *
 */
int width = 320;
int height = 240;
char *engine = "sdl";

void parse(EXML *exml, Etk_Document *doc);

void help(void)
{
	printf("etk2_testsuite [OPTIONS] FILE\n");
	printf("-e Engine\n");
	printf("-w Width\n");
	printf("-h Height\n");
	printf("FILE Etk2 XML file\n");
}

void tag_create(char *tag, EXML *exml, Ekeko_Object *parent)
{
	if (!strcmp(tag, "rect"))
	{
		Etk_Rect *r;
		r = etk_rect_new(parent);
		printf("Creating a rect\n");
	}
	else if (!strcmp(tag, "image"))
	{
		printf("Creating an image\n");
	}
}

void parse(EXML *exml, Etk_Document *doc)
{
	char *tag;

	tag = exml_down(exml);
	while (tag)
	{
		printf("found tag %s\n", tag);
		tag_create(tag, exml, (Ekeko_Object *)doc);
		/* siblings */
		tag = exml_next(exml);
		parse(exml, doc);
	}
	exml_up(exml);
}

int main(int argc, char **argv)
{
	EXML *exml;
	EXML_Node *n;
	Etk_Document *doc;

	exml = exml_new();
	if (!exml_file_read(exml, argv[1]))
	{
		printf("no file\n");
		return -1;
	}
	n = exml_get(exml);
	if (strcmp(n->tag, "etk"))
	{
		printf("no etk file\n");
	}
	/* create the context */
	etk_init();
	doc = etk_document_new(engine, width, height);
	/* parse the file */
	parse(exml, doc);
	etk_loop();
	etk_shutdown();
	exml_destroy(exml);

	return 0;
}
