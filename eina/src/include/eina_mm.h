#ifndef EINA_MM_H_
#define EINA_MM_H_

typedef struct _Eina_Mm Eina_Mm;

EAPI Eina_Mm * eina_mm_new(void *chunk, unsigned int size, int type, ...);
void * eina_mm_alloc(Eina_Mm *m, unsigned int size);
void * eina_mm_realloc(Eina_Mm *m, void *data, unsigned int size);
void eina_mm_free(Eina_Mm *m, void *data);

#endif /*EINA_MM_H_*/
