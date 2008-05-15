#ifndef ESVG_DOCUMENT_H_
#define ESVG_DOCUMENT_H_

typedef struct _ESVG_Document ESVG_Document;

struct _ESVG_Document
{
	/* inheritance */
	Eobj_Object object;
	ESVG_Element *top;
	char *title;
	char *referrer;
	char *domain;
	char *URL;
};


#endif /*ESVG_DOCUMENT_H_*/
