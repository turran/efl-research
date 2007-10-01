#ifndef _ENESIM_H
#define _ENESIM_H

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
 * @image html overview.png 
 * @section Implementation Implementation
 * @image html implementation.png 
 * 
 * @file
 * @brief Enesim API
 * Enesim is a library that ...
 * @image html overview.png
 * 
 * @defgroup Enesim API
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

/*
 * @defgroup Enesim API
 *
 * @{
 *
 */
enum
{
	ENESIM_CMD_MOVE_TO,
	ENESIM_CMD_LINE_TO,
	ENESIM_CMD_END,
	ENESIM_CMDS
};

/**
 * TODO normalize this errors
 *
 */
enum
{
	ENESIM_ERROR_NONE 	= 1,
	ENESIM_ERROR_INVAL,
	ENESIM_ERROR_IO,
	ENESIM_ERRORS
};

/**
 * @defgroup Enesim_Component_Group Component
 * @{
 */
typedef struct _Enesim_Component 		Enesim_Component;
typedef struct _Enesim_Component_Reader 	Enesim_Component_Reader;
EAPI void enesim_component_delete(Enesim_Component *c);
EAPI int enesim_component_source_set(Enesim_Component *c, Enesim_Component *src);
/**
 * @defgroup Enesim_Component_Transform_Group Transform Component
 * @brief This Component applies a transformation matrix.
 * Reads vertex values from another component that is linked with the XXX
 * function and applies a transformation matrix to them.
 * @{
 */
EAPI Enesim_Component * enesim_transform_new(void);
EAPI void enesim_transform_matrix_set(Enesim_Component *c, float *matrix);
/**
 * @}
 * @defgroup Enesim_Component_Csv_Source_Group CSV Source Component
 * @brief This Component reads data from a csv (Comma Separated Values) file.
 * @{
 */
EAPI Enesim_Component * enesim_source_csv_new(void);
EAPI int enesim_source_csv_file_set(Enesim_Component *c, const char *path);
/** 
 *  @}
 *  @}
 * @defgroup Enesim_Component_Reader_Group Reader
 * @{
 */
EAPI Enesim_Component_Reader * enesim_reader_new(Enesim_Component *c);
EAPI void enesim_reader_rewind(Enesim_Component_Reader *r);
EAPI int enesim_reader_vertex_get(Enesim_Component_Reader *r, float *x, float *y);
/** @} */
/** @} */

#if 0
typedef struct _Enesim_Path Enesim_Path;
EAPI Enesim_Path * enesim_path_new(int num_vertices);
EAPI void enesim_path_delete(Enesim_Path *p);
EAPI void enesim_path_transform(Enesim_Path *s, Enesim_Path *d, float *tm);
/* for now */
EAPI void enesim_path_vertex_add(Enesim_Path *p, float x, float y);
EAPI void enesim_path_vertex_get(Enesim_Path *p, float *x, float *y);
EAPI void enesim_path_rewind(Enesim_Path *p);
#endif
#endif
