/*
 * etk2_document.h
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */

#ifndef ETK2_DOCUMENT_H_
#define ETK2_DOCUMENT_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define ETK_DOCUMENT_SIZE_CHANGED "sizeChanged"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Property_Id ETK_DOCUMENT_SIZE;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Etk_Document_Private Etk_Document_Private;
struct _Etk_Document
{
	Ekeko_Object parent;
	Etk_Document_Private *private;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Ekeko_Type *etk_document_type_get(void);
EAPI Etk_Document * etk_document_new(const char *name, int w, int h);
EAPI Etk_Canvas * etk_document_canvas_get(Etk_Document *d);
EAPI void etk_document_size_get(Etk_Document *d, int *w, int *h);
EAPI void etk_document_resize(Etk_Document *d, int w, int h);
#endif /* ETK2_DOCUMENT_H_ */
