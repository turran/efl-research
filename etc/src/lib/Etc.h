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
 * @mainpage Etc
 * @section intro Introduction
 * Etc is a library that ...
 * @image html overview.png
 * 
 * @file
 * @brief Etc API
 * @defgroup Etc API
 *
 * @{
 * @defgroup Etc_Scanline_Group Scanline
 * @{
 */

typedef struct _Etc_Scanline 	Etc_Scanline;

EAPI Etc_Scanline 	*etc_scanline_new(void);
EAPI void 		etc_scanline_delete(Etc_Scanline *sl);

/** @} */

/**
 * @defgroup Etc_Rasterizer_Group Rasterizer
 * @{
 */

typedef struct _Etc_Rasterizer 	Etc_Rasterizer;

EAPI void etc_rasterizer_vertex_add(Etc_Rasterizer *r, float x, float y);
EAPI void etc_rasterizer_generate(Etc_Rasterizer *r, Etc_Scanline *sl);
EAPI void etc_rasterizer_boundings_set(Etc_Rasterizer *r, int x, int y, int w, int h);
EAPI Etc_Rasterizer * etc_rasterizer_new(void);

/** @} */

#endif
