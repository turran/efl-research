#ifndef ESVG_PARSER_H_
#define ESVG_PARSER_H_

/* 
 * define attribute collections
 * create an object for each element
 * some attributes accumulate (transform) and others replace (color) 
 * 
 */
EAPI ESVG_Document * esvg_document_load(const char *file, unsigned int w, unsigned int h, ESVG_Engine_Type type, void *engine_data);

#endif /*ESVG_PARSER_H_*/
