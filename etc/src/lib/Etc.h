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

typedef struct _Etc_Scanline Etc_Scanline;

EAPI Etc_Scanline * etc_scanline_new(void);
EAPI void etc_scanline_edge_reset(Etc_Scanline *s);
EAPI void etc_scanline_edge_add(Etc_Scanline *s, float x0, float y0, float x1, float y1);
EAPI void etc_scanline_vertex_add(Etc_Scanline *s, float x, float y);
EAPI void etc_scanline_generate_all(Etc_Scanline *s);

#endif
