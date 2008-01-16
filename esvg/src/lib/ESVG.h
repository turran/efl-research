#ifndef _ESVG_H
#define _ESVG_H

/**
 * TODO split the code in two, the drawing and the parser =)
 */
typedef struct _ESVG ESVG;
EAPI void esvg_file_save(const char *file);
EAPI ESVG* esvg_file_load(const char *file);

#endif
