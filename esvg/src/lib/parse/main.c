#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static ESVG_Document * _document_new(void)
{
	ESVG_Document *svg;
	
	svg = calloc(1, sizeof(ESVG_Document));
	return svg;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Document_Element *esvg_elements[ESVG_ELEMENTS] = {
	[ESVG_ELEMENT_SVG] = &element_document,
	[ESVG_ELEMENT_G] = &element_group,
};

char *element_tags[ESVG_ELEMENTS] = {
	[ESVG_ELEMENT_SVG] = "svg",
	[ESVG_ELEMENT_RECT] = "rect",
	[ESVG_ELEMENT_G] = "g",
	[ESVG_ELEMENT_POLYLINE] = "polyline",
	[ESVG_ELEMENT_POLYGON] = "polygon",
};

/* parse every child object */
void element_child_parse(ESVG_Document *ed, ESVG_Document_Element *ede)
{
	EXML_Node *n;
	char *tag;
	
	if (!ede) return;
	
	n = exml_get(ed->xml);
	tag = exml_down(ed->xml);
	while (tag != NULL)
	{
		int i;
		
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
				ESVG_Document_Element *el;
				
				printf("parsing %s\n", tag);
				el = esvg_elements[index];
				el->parser(ed);
			}
		}
		tag = exml_next(ed->xml);
	}
	exml_goto_node(ed->xml, n);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG * esvg_document_load(const char *file, ESVG_Engine_Type type, void *engine_data)
{
	ESVG_Document *svg;
	EXML *xml;
	EXML_Node *n;
	char *tag;
	
	xml = exml_new();
	if (!exml_file_read(xml, file))
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
	svg = _document_new();
	svg->xml = xml;
	svg->type = type;
	svg->engine_data = engine_data;
	esvg_elements[ESVG_ELEMENT_SVG]->parser(svg);
	return NULL;
}
