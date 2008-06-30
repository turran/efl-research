#ifndef EINA_MM_H_
#define EINA_MM_H_

typedef struct _Eina_Mp Eina_Mp;

EAPI int eina_mp_init(void);
EAPI int eina_mp_shutdown(void);
EAPI void eina_mp_delete(Eina_Mp *mp);
EAPI Eina_Mp * eina_mp_new_from_buffer(const char *name, void *buffer,
		unsigned int size, const char *options, ...);
EAPI Eina_Mp * eina_mp_new(const char *name, unsigned int size, const char 
		*options, ...);
EAPI void * eina_mp_realloc(Eina_Mp *mp, void *element, unsigned int size);
EAPI void * eina_mp_alloc(Eina_Mp *mp, unsigned int size);
EAPI void eina_mp_free(Eina_Mp *mp, void *element);

#endif /*EINA_MM_H_*/
