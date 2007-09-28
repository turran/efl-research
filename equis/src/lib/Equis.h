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
 * @mainpage Equis
 * @section intro Introduction
 * Equis is a library that ...
 * @image html overview.png 
 * 
 * @file
 * @brief Equis API
 * @defgroup Equis API
 *
 * @{
 *
 */
enum
{
	EQUIS_CMD_MOVE_TO,
	EQUIS_CMD_LINE_TO,
	EQUIS_CMD_END,
	EQUIS_CMDS
};

/**
 * TODO normalize this errors
 *
 */
enum
{
	EQUIS_ERROR_NONE 	= 1,
	EQUIS_ERROR_INVAL,
	EQUIS_ERROR_IO,
	EQUIS_ERRORS
};

/**
 * @defgroup Equis_Component_Group Component
 * @{
 */
typedef struct _Equis_Component 	Equis_Component;
typedef struct _Equis_Component_Reader 	Equis_Component_Reader;
EAPI void equis_component_delete(Equis_Component *c);
EAPI int equis_component_source_set(Equis_Component *c, Equis_Component *src);
/**
 * @defgroup Equis_Component_Transform_Group Transform Component
 * @brief This Component applies a transformation matrix.
 * Reads vertex values from another component that is linked with the XXX
 * function and applies a transformation matrix to them.
 * @{
 */
EAPI Equis_Component * equis_transform_new(void);
EAPI void equis_transform_matrix_set(Equis_Component *c, float *matrix);
/**
 * @}
 * @defgroup Equis_Component_Csv_Source_Group CSV Source Component
 * @brief This Component reads data from a csv (Comma Separated Values) file.
 * @{
 */
EAPI Equis_Component * equis_source_csv_new(void);
EAPI int equis_source_csv_file_set(Equis_Component *c, const char *path);
/** 
 *  @}
 *  @}
 * @defgroup Equis_Component_Reader_Group Reader
 * @{
 */
EAPI Equis_Component_Reader * equis_reader_new(Equis_Component *c);
EAPI void equis_reader_rewind(Equis_Component_Reader *r);
EAPI int equis_reader_vertex_get(Equis_Component_Reader *r, float *x, float *y);
/** @} */
/** @} */

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
