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

enum
{
	EQUIS_CMD_MOVE_TO,
	EQUIS_CMD_LINE_TO,
	EQUIS_CMD_END,
};


/* component */
typedef struct _Equis_Component 	Equis_Component;
typedef struct _Equis_Component_Reader 	Equis_Component_Reader;
EAPI void equis_component_delete(Equis_Component *c);
/* transform */

/* source csv */
EAPI Equis_Component * equis_source_csv_new(void);
EAPI int equis_source_csv_file_set(Equis_Component *c, const char *path);
/* reader */
EAPI Equis_Component_Reader * equis_reader_new(Equis_Component *c);
EAPI void equis_reader_rewind(Equis_Component_Reader *r);
EAPI int equis_reader_vertex_get(Equis_Component_Reader *r, float *x, float *y);

#if 0
typedef struct _Equis_Path Equis_Path;
EAPI Equis_Path * equis_path_new(int num_vertices);
EAPI void equis_path_delete(Equis_Path *p);
EAPI void equis_path_transform(Equis_Path *s, Equis_Path *d, float *tm);
/* for now */
EAPI void equis_path_vertex_add(Equis_Path *p, float x, float y);
EAPI void equis_path_vertex_get(Equis_Path *p, float *x, float *y);
EAPI void equis_path_rewind(Equis_Path *p);
#endif
#endif
