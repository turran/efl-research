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
	[ESVG_ELEMENT_DOCUMENT] = &document_element,
};
/* given a tag, parse it finding the element that
 * parse that tag
 */
void element_parse(ESVG_Document *ed, const char *tag)
{
	int i;
	
	for (i = 0; i < ESVG_ELEMENTS; i++)
	{
		ESVG_Document_Element *el;
		
		el = esvg_elements[i];
		
		printf("%d\n", el);
		if (!strcmp(tag, el->tag));
			el->parser(ed);
	}
}
/* parse every child object */
void element_child_parse(ESVG_Document_Element *ed)
{
	
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG * esvg_document_load(const char *file)
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
	printf("%p\n", n);
	if (strcmp(n->tag, "svg"))
	{
		printf("no svg\n");
		return NULL;
	}
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("empty tags\n");
		return NULL;
	}
	svg = _document_new();
	svg->xml = xml;
	element_parse(svg, "svg");
	return NULL;
}
