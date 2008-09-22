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
extern Eina_Error EMAGE_ERROR_PROVIDER;
extern Eina_Error EMAGE_ERROR_FORMAT;
extern Eina_Error EMAGE_ERROR_SIZE;

typedef enum _Emage_Provider_Type
{
		EMAGE_PROVIDER_SW,
		EMAGE_PROVIDER_HW,
} Emage_Provider_Type;

typedef struct _Emage_Provider
{
	const char *name;
	Emage_Provider_Type type;
	Eina_Bool (*loadable)(const char *file);
	Eina_Bool (*info_get)(const char *file, int *w, int *h, Enesim_Surface_Format *sfmt);
	Eina_Bool (*load)(const char *file, Enesim_Surface *s);
} Emage_Provider;

typedef void (*Emage_Load_Callback)(Enesim_Surface *s, void *data, int error);

EAPI int emage_init(void);
EAPI void emage_shutdown(void);

EAPI Eina_Bool emage_info_load(const char *file, int *w, int *h, Enesim_Surface_Format *sfmt);
EAPI Eina_Bool emage_load(const char *file, Enesim_Surface **s);
EAPI void emage_load_async(const char *file, Enesim_Surface **s, Emage_Load_Callback cb, void *data);
EAPI Enesim_Surface * emage_save(const char *file);
//EAPI Enesim_Surface * emage_save_async(const char *file, Emage_Load_Callback);
EAPI int emage_provider_register(Emage_Provider *);

#endif
