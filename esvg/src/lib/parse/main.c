#include "ESVG.h"
#include "esvg_private.h"

struct _ESVG
{
	EXML *xml;
};

ESVG_Element esvg_elements[ESVG_ELEMENTS];

static ESVG * esvg_new(void)
{
	ESVG *svg;
	
	svg = calloc(1, sizeof(ESVG));
	return svg;
}

void element_parse(ESVG_Element *e, const char *tag)
{
	int el;
	
	for (el = 0; el < ESVG_ELEMENTS; el++)
	{
		if (!strcmp(tag, esvg_elements[el].tag));
			//esvg_elements[el].parser();
	}
}

EAPI void esvg_file_save(const char *file)
{
	
}

EAPI ESVG* esvg_file_load(const char *file)
{
	ESVG *svg;
	EXML *xml;
	EXML_Node *n;
	char *tag;
	
	xml = exml_new();
	if (!exml_file_read(xml, file))
	{
		printf("cant read file??\n");
	}
	n = exml_get(xml);
	if (strcmp(n->tag, "svg"))
	{
		printf("no svg\n");
		return -1;
	}
	if ((tag = exml_down(xml)) == NULL)
	{
		printf("empty tags\n");
		return -2;
	}
	svg->xml = xml;
	//element_parse(esvg_elements[ESVG_ELEMENT_DOCUMENT], svg);
	return svg;
}
