#include "ESVG.h"
#include "esvg_private.h"

void document_parse(ESVG *svg)
{
	
}

ESVG_Element document_element = {
	.tag = "svg",
	.parser = &document_parse,
	.children = { ESVG_ELEMENT_DOCUMENT, ESVG_ELEMENT_GROUP, ESVG_ELEMENT_POLYGON},
};