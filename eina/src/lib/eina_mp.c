#include "Eina.h"
#include "eina_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Eina_List *_modules;
static int _init_count = 0;

struct _Eina_Mp
{
#ifdef DEBUG
	unsigned int magic;
#endif
	Eina_Module *module;
	Eina_Mp_Backend *backend;
	void *backend_data;
};
static Eina_Mp * _new_from_buffer(const char *name, void *buffer,
		unsigned int size, const char *options, va_list args)
{
	Eina_List *l;
	
	/* load the module with filename == name */
	for (l = _modules; l; l = eina_list_next(l))
	{
		Eina_Module *m;

		m = eina_list_data(l);
		/* check if the requested module name exists */
		if (!strncmp(eina_module_name_get(m), name, strlen(name)))
		{
			Eina_Mp *mp;

			mp = malloc(sizeof(Eina_Mp));
			eina_module_load(m);
			mp->module = m;
			mp->backend = eina_module_symbol_get(m, "mp_backend");
			mp->backend_data = mp->backend->init(size, options, args);

			return mp;
		}
	}
	return NULL;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI int eina_mp_init(void)
{
	/* get all the modules */
	_modules = eina_module_list_get("/usr/local/lib/eina/mm_policies", 0, NULL, NULL);	
}
/**
 * 
 */
EAPI void eina_mp_shutdown(void)
{
	/* remove the list of modules */
	eina_module_list_delete(_modules);
}
/**
 * 
 */
EAPI Eina_Mp * eina_mp_new_from_buffer(const char *name, void *buffer,
		unsigned int size, const char *options, ...)
{
	Eina_Mp *mp;
	va_list args;
	
	assert(name);
	assert(buffer);
	
	va_start(args, options);
	mp = _new_from_buffer(name, buffer, size, options, args);
	va_end(args);
	
	return mp;	
}
/**
 * 
 */
EAPI Eina_Mp * eina_mp_new(const char *name, unsigned int size, const char 
		*options, ...)
{
	Eina_Mp *mp;
	void *buffer;
	va_list args;
	
	assert(name);
	
	buffer = malloc(sizeof(char) * size);
	va_start(args, options);
	mp = _new_from_buffer(name, buffer, size, options, args);
	va_end(args);
	
	return mp;
}
/**
 * 
 */
EAPI void eina_mp_delete(Eina_Mp *mp)
{
	Eina_List *l;
	
	assert(mp);
	
	mp->backend->shutdown(mp->backend_data);
	eina_module_unload(mp->module);
	free(mp);
}
/**
 * 
 */
EAPI void * eina_mp_pop(Eina_Mp *mp, unsigned int size)
{
	assert(mp);
	assert(mp->backend->pop);
	
	return mp->backend->pop(mp->backend_data, size);
}
/**
 * 
 */
EAPI void eina_mp_push(Eina_Mp *mp, void *element)
{
	assert(mp);
	assert(mp->backend->push);
	
	mp->backend->push(mp->backend_data, element);
}
