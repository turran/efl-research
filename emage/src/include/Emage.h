#ifndef EMAGE_H
#define EMAGE_H

#include <Eina.h>
#include <Enesim.h>

/**
 * @mainpage Emage
 * @section intro Introduction
 * Emage is a library that loads several types of image formats
 *
 */

extern Eina_Error EMAGE_ERROR_EXIST;
extern Eina_Error EMAGE_ERROR_LOADER;
extern Eina_Error EMAGE_ERROR_FORMAT;
extern Eina_Error EMAGE_ERROR_SIZE;

typedef struct _Emage_Provider
{
	const char *name;
	Eina_Bool (*load)(const char *file, Enesim_Surface **s);
} Emage_Provider;

typedef void (*Emage_Load_Callback)(Enesim_Surface *s, void *data, int error);

EAPI int emage_init(void);
EAPI void emage_shutdown(void);

EAPI void emage_info_load(const char *file, Enesim_Surface **s);
EAPI Eina_Bool emage_load(const char *file, Enesim_Surface **s);
EAPI void emage_load_async(const char *file, Enesim_Surface **s, Emage_Load_Callback cb, void *data);
EAPI Enesim_Surface * emage_save(const char *file);
//EAPI Enesim_Surface * emage_save_async(const char *file, Emage_Load_Callback);
EAPI int emage_provider_register(Emage_Provider *);

#endif
