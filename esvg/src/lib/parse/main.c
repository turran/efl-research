#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static ESVG_Parser * _parser_new(void)
{
	ESVG_Parser *parser;
	
	parser = calloc(1, sizeof(ESVG_Parser));
	parser->engine = esvg_engine_new();
	
	return parser;
}
static ESVG_Parser * _document_del(ESVG_Parser *d)
{

}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element *esvg_elements[ESVG_ELEMENTS] = {
	[ESVG_ELEMENT_SVG] = &element_svg,
	[ESVG_ELEMENT_G] = &element_group,
	[ESVG_ELEMENT_RECT] = &element_rect,
};

char *element_tags[ESVG_ELEMENTS] = {
	[ESVG_ELEMENT_SVG] = "svg",
	[ESVG_ELEMENT_RECT] = "rect",
	[ESVG_ELEMENT_G] = "g",
	[ESVG_ELEMENT_POLYLINE] = "polyline",
	[ESVG_ELEMENT_POLYGON] = "polygon",
};

/* parse every child object */
void element_child_parse(ESVG_Parser *ed, ESVG_Document_Element *ede, ESVG_Element *last)
{
	EXML_Node *n;
	char *tag;
	
	if (!ede) return;
	
	n = exml_get(ed->xml);
	tag = exml_down(ed->xml);
	while (tag != NULL)
	{
		int i;
		
		printf("tag %s found\n", tag);
		/* iterate over the children */
		for (i = 0; i < ESVG_ELEMENTS; i++)
		{
			int index;
			
			index = ede->children[i]; 
			if (index == ESVG_ELEMENT_UNKNOWN)
				continue;
			if (!element_tags[index])
			{
				printf("unsupported tag %s\n", tag);
				continue;
			}
			if (!strcmp(element_tags[index], tag))
			{
				ESVG_Document_Element *del;
				ESVG_Element *new;
				
				printf("parsing %s\n", tag);
				del = esvg_elements[index];
				/* TODO fix this */
				if (!del->parser(ed, last, &new))
					break;
				if (last)
					esvg_container_add(ESVG_CONTAINER(last), new);
				/* now parse the children */
				element_child_parse(ed, del, new);
			}
		}
		tag = exml_next(ed->xml);
	}
	exml_goto_node(ed->xml, n);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG_Svg * esvg_document_load(const char *file, unsigned int w, unsigned int h, ESVG_Engine_Type type, void *engine_data)
{
	ESVG_Engine *engine;
	ESVG_Parser *parser;
	ESVG_Svg *svg = NULL;
	EXML *xml;
	EXML_Node *n;
	char *tag;
	
	xml = exml_new();
	if (!exml_file_read(xml, (char *)file))
	{
		printf("cant read file??\n");
		return NULL;
	}
	n = exml_get(xml);
	if (strcmp(n->tag, "svg"))
	{
		printf("no svg\n");
		return NULL;
	}
	parser = _parser_new();
	parser->xml = xml;
	esvg_engine_set(parser->engine, type, engine_data);
	esvg_engine_output_size_set(parser->engine, w, h);
	/* lets simulate the parsing of the first element */
	esvg_elements[ESVG_ELEMENT_SVG]->parser(parser, NULL, (ESVG_Element **)&svg);
	if (svg)
		element_child_parse(parser, esvg_elements[ESVG_ELEMENT_SVG], ESVG_ELEMENT(svg));
	return svg;
}
