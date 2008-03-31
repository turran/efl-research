#ifndef EINA_MODULE_H_
#define EINA_MODULE_H_

typedef struct _Eina_Module Eina_Module;

typedef void (*Eina_Module_Load_Cb)(Eina_Module *m, void *data);

EAPI Eina_Module * eina_module_load(const char *name);
EAPI eina_module_load_all(const char *dir, Eina_Module_Load_Cb cb, void *data);
EAPI void eina_module_unload(Eina_Module *m);
EAPI void * eina_module_symbol_get(Eina_Module *m, const char *symbol);

#endif /*EINA_MODULE_H_*/
