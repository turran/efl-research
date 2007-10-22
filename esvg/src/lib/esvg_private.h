#ifndef _ESVG_PRIVATE_H
#define _ESVG_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include "Enesim.h"
#include "EXML.h"

enum
{
	ESVG_ELEMENT_GROUP,
	ESVG_ELEMENT_POLYLINE,
	ESVG_ELEMENT_POLYGON,
	ESVG_ELEMENTS
};

enum
{

	ESVG_ATTRIBUTES
};


typedef void (*ESVG_Element_Parser)(void);
typedef void (*ESVG_Attribute_Parser)(void);

typedef struct _ESVG_Element
{
	const char *name;
	ESVG_Element_Parser *parser;	
} ESVG_Element;

typedef struct _ESVG_Attribute
{
	const char *name;
	ESVG_Attribute_Parser *parser;	
} ESVG_Attribute;

ESVG_Element esvg_elements[ESVG_ELEMENTS] = {
	.[ESVG_ELEMENT_GROUP] = {
		.name = "g",
	},
	.[ESVG_ELEMENT_POLYLINE] = {
		.name = "polyline",
	},
	.[ESVG_ELEMENT_POLYGON] = {
		.name = "polygon",
	},
};

#endif
