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

typedef struct _Etc_Rasterizer 	Etc_Rasterizer;
typedef struct _Etc_Scanline 	Etc_Scanline;

EAPI void etc_rasterizer_vertex_add(Etc_Rasterizer *r, float x, float y);
EAPI void etc_rasterizer_generate(Etc_Rasterizer *r, Etc_Scanline *s);
EAPI Etc_Rasterizer * etc_rasterizer_new(void);


#if 0
EAPI void etc_scanline_edge_reset(Etc_Scanline *s);
EAPI void etc_scanline_edge_add(Etc_Scanline *s, float x0, float y0, float x1, float y1);
#endif

#endif
