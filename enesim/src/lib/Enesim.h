#ifndef _ETC_H
#define _ETC_H

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/**
 * @mainpage Enesim
 * @section intro Introduction
 * Enesim is a library that ...
 * @image html overview.png
 * 
 * @file
 * @brief Enesim API
 * @defgroup Enesim API
 *
 * @{
 * @defgroup Enesim_Scanline_Group Scanline
 * @{
 */

typedef struct _Enesim_Scanline 	Enesim_Scanline;

EAPI Enesim_Scanline 	*enesim_scanline_new(void);
EAPI void 		enesim_scanline_delete(Enesim_Scanline *sl);

/** @} */

/**
 * @defgroup Enesim_Rasterizer_Group Rasterizer
 * @{
 */

typedef struct _Enesim_Rasterizer 	Enesim_Rasterizer;

EAPI void enesim_rasterizer_vertex_add(Enesim_Rasterizer *r, float x, float y);
EAPI void enesim_rasterizer_generate(Enesim_Rasterizer *r, Enesim_Scanline *sl);
EAPI void enesim_rasterizer_boundings_set(Enesim_Rasterizer *r, int x, int y, int w, int h);
EAPI Enesim_Rasterizer * enesim_rasterizer_new(void);

/** @} */

/**
 * @defgroup Enesim_Renderer_Group Renderer
 * @{
 */
typedef struct _Enesim_Renderer 	Enesim_Renderer;
EAPI Enesim_Renderer * enesim_fill_color_new(void);
EAPI void enesim_fill_color_color_set(Enesim_Renderer *r);
/** @} */

/**
 * @defgroup Enesim_Surface_Group Surface
 * @{
 */
typedef unsigned int 		DATA32;
typedef unsigned short int 	DATA16;
typedef unsigned char 		DATA8;
typedef struct _Enesim_Surface 	Enesim_Surface;

/** @} */

#endif
