#ifndef EINA_MM_H_
#define EINA_MM_H_

typedef struct _Eina_Mp Eina_Mp;

EAPI int eina_mp_init(void);
EAPI void eina_mp_shutdown(void);
EAPI void eina_mp_delete(Eina_Mp *mp);
EAPI Eina_Mp * eina_mp_new_from_buffer(const char *name, void *buffer,
		unsigned int size, const char *options, ...);
EAPI Eina_Mp * eina_mp_new(const char *name, unsigned int size, const char 
		*options, ...);
#endif /*EINA_MM_H_*/
