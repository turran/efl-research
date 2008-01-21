#ifndef _ESVG_PRIVATE_H
#define _ESVG_PRIVATE_H

#include "esvg_draw_private.h"


/* old code */
#if 0

/* 
 * define attribute collections
 * create an object for each element
 * some attributes accumulate (transform) and others replace (color) 
 * 
 */

typedef struct _ESVG_Context ESVG_Context;

typedef struct _ESVG_Element
{
	ESVG_Context dc;
	ESVG_Context dc_merged;
} ESVG_Element;

typedef struct _ESVG_Shape
{
	ESVG_Element element;
} ESVG_Shape;

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


typedef void (*ESVG_Element_Parser)(ESVG *svg);
typedef void (*ESVG_Attribute_Parser)(void);

#if 0
typedef struct _ESVG_Element
{
	const char *tag;
	const int children[ESVG_ELEMENTS];
	ESVG_Element_Parser parser;
} ESVG_Element;

typedef struct _ESVG_Attribute
{
	const char *name;
	ESVG_Attribute_Parser parser;	
} ESVG_Attribute;

extern ESVG_Element esvg_elements[ESVG_ELEMENTS];
#endif
#endif
#endif

