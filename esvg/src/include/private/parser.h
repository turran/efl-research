#ifndef PARSER_H_
#define PARSER_H_

enum
{
	ESVG_ELEMENT_DOCUMENT,
	ESVG_ELEMENT_GROUP,
	ESVG_ELEMENT_POLYLINE,
	ESVG_ELEMENT_POLYGON,
	ESVG_ELEMENTS
};

enum
{

	ESVG_ATTRIBUTES
};


typedef struct _ESVG_Document
{
	ESVG *canvas;
	EXML *xml;
} ESVG_Document;

typedef Eina_Bool (*ESVG_Document_Element_Parser)(ESVG_Document *svg);
typedef void (*ESVG_Document_Attribute_Parser)(void);

typedef struct _ESVG_Document_Element
{
	const char *tag;
	const int children[ESVG_ELEMENTS];
	ESVG_Document_Element_Parser parser;
} ESVG_Document_Element;


typedef struct _ESVG_Attribute
{
	const char *name;
	ESVG_Document_Attribute_Parser parser;	
} ESVG_Attribute;

extern ESVG_Document_Element *esvg_elements[ESVG_ELEMENTS];
extern ESVG_Document_Element document_element;

#endif /*PARSER_H_*/
