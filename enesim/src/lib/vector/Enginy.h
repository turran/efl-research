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
 * @mainpage Enginy
 * @section Intro Introduction
 * Enginy is a library that ...
 * @image html overview.png 
 * @section Implementation Implementation
 * @image html implementation.png 
 * 
 * @file
 * @brief Enginy API
 * @defgroup Enginy API
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
 * @defgroup Enginy_Component_Group Component
 * @{
 */
typedef struct _Enginy_Component 	Enginy_Component;
typedef struct _Enginy_Component_Reader 	Enginy_Component_Reader;
EAPI void enginy_component_delete(Enginy_Component *c);
EAPI int enginy_component_source_set(Enginy_Component *c, Enginy_Component *src);
/**
 * @defgroup Enginy_Component_Transform_Group Transform Component
 * @brief This Component applies a transformation matrix.
 * Reads vertex values from another component that is linked with the XXX
 * function and applies a transformation matrix to them.
 * @{
 */
EAPI Enginy_Component * enginy_transform_new(void);
EAPI void enginy_transform_matrix_set(Enginy_Component *c, float *matrix);
/**
 * @}
 * @defgroup Enginy_Component_Csv_Source_Group CSV Source Component
 * @brief This Component reads data from a csv (Comma Separated Values) file.
 * @{
 */
EAPI Enginy_Component * enginy_source_csv_new(void);
EAPI int enginy_source_csv_file_set(Enginy_Component *c, const char *path);
/** 
 *  @}
 *  @}
 * @defgroup Enginy_Component_Reader_Group Reader
 * @{
 */
EAPI Enginy_Component_Reader * enginy_reader_new(Enginy_Component *c);
EAPI void enginy_reader_rewind(Enginy_Component_Reader *r);
EAPI int enginy_reader_vertex_get(Enginy_Component_Reader *r, float *x, float *y);
/** @} */
/** @} */

#if 0
typedef struct _Enginy_Path Enginy_Path;
EAPI Enginy_Path * enginy_path_new(int num_vertices);
EAPI void enginy_path_delete(Enginy_Path *p);
EAPI void enginy_path_transform(Enginy_Path *s, Enginy_Path *d, float *tm);
/* for now */
EAPI void enginy_path_vertex_add(Enginy_Path *p, float x, float y);
EAPI void enginy_path_vertex_get(Enginy_Path *p, float *x, float *y);
EAPI void enginy_path_rewind(Enginy_Path *p);
#endif
#endif
