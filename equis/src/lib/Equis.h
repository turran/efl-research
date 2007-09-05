#ifndef _EQUIS_H
#define _EQUIS_H

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
 *
 *
 */

typedef struct _Equis_Generator Equis_Generator; /**< */

EAPI void equis_path_move_to(Equis_Generator *g, eix x, eix y);
EAPI void equis_path_line_to(Equis_Generator *g, eix x, eix y);
EAPI void equis_path_close(Equis_Generator *g);

/**
 *
 */
enum Cmd
{
	TEST
};

EAPI int equis_generator_vertex_get(Equis_Generator *g, eix *x, eix *y);

#endif
