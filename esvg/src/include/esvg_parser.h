#ifndef ESVG_PARSER_H_
#define ESVG_PARSER_H_

/* 
 * define attribute collections
 * create an object for each element
 * some attributes accumulate (transform) and others replace (color) 
 * 
 */

ESVG * esvg_document_load(const char *file);

#endif /*ESVG_PARSER_H_*/
