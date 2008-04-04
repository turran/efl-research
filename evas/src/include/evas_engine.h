#ifndef EVAS_ENGINE_H_
#define EVAS_ENGINE_H_

/**
 * @defgroup Evas_Engine_Group Engines
 * @{
 */
typedef struct _Evas_Engine Evas_Engine; /**< */
typedef struct _Evas_Engine_Info Evas_Engine_Info; /**< */
EAPI Evas_Engine_Info *evas_engine_info_get(const Evas *e);
EAPI void              evas_engine_info_set(Evas *e, Evas_Engine_Info *info);

#ifdef EVAS_MODULE

EAPI int evas_engine_register(const char *name, Evas_Engine_Func *fnc);

#endif

/** 
 * @}
 */

#endif /*EVAS_ENGINE_H_*/
