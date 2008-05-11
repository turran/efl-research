#include "Eina.h"
#include "eina_private.h"

#include <dirent.h>
#include <string.h>
#include <dlfcn.h>

/**
 * TODO also store the referenced file name
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
struct _Eina_Module
{
	void *handle;
	char *name;
};

#define MODULE_EXTENSION ".so"
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Module * eina_module_load(const char *name)
{
	Eina_Module *m;
	void *dl_handle;
	
	dl_handle = dlopen(name, RTLD_LAZY);
	if (!dl_handle) return NULL;
	
	m = malloc(sizeof(Eina_Module));
	m->handle = dl_handle;
	
	return m;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eina_module_load_all(const char *dir, Eina_Module_Load_Cb cb, void *data)
{
	struct dirent *de;
	DIR *d;
	
	d = opendir(dir);
	if (!d) return;
	
	while (de = readdir(d))
	{
		int length;
		
		length = strlen(de->d_name);
		if (length < strlen(MODULE_EXTENSION) + 1) /* x.so */
			continue;
		if (!strcmp(de->d_name + length - strlen(MODULE_EXTENSION), MODULE_EXTENSION))
		{
			char file[PATH_MAX];
			Eina_Module *m;
			
			snprintf(file, PATH_MAX, "%s/%s", dir, de->d_name);
			m = eina_module_load(file);
			if (!m) continue;
			
			cb(m, data);
		}
	}
	closedir(d);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eina_module_unload(Eina_Module *m)
{
	dlclose(m->handle);
	free(m);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eina_module_symbol_get(Eina_Module *m, const char *symbol)
{
	return dlsym(m->handle, symbol);	
}
