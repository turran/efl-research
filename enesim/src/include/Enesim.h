#ifndef _ENESIM_H
#define _ENESIM_H

#include "enesim_rectangle.h"

/**
 * @mainpage Enesim
 * @section intro Introduction
 * Enesim is a library that ...
 * @image html vector.png
 * The raster sub-library is ...
 * @image html raster.png 
 * 
 * @file
 * @brief Enesim API
 * @defgroup Enesim_Group API
 * @{
 *
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
 * @defgroup Enesim_Raster_Group Raster
 * @{
 */
typedef unsigned int 		DATA32;
typedef unsigned short int 	DATA16;
typedef unsigned char 		DATA8;


/**
 * @defgroup Enesim_Scanline_Group Scanline
 * @{
 */

/**
 * To be documented
 * FIXME: To be fixed
 */
typedef struct _Enesim_Scanline_Alias
{
	int y;
	int x;
	int w;
} Enesim_Scanline_Alias;

/**
 * To be documented
 * FIXME: To be fixed
 */
typedef struct _Enesim_Scanline_Mask
{
	int y;
	int x;
	int w;
	DATA8 *coverages;
} Enesim_Scanline_Mask;

enum {
	ENESIM_SCANLINE_ALIAS  	    = (1 << 0),
	ENESIM_SCANLINE_ANTIALIAS   = (1 << 1),
	ENESIM_SCANLINE_MASK        = (1 << 2),
	ENESIM_SCANLINES
};

typedef void (*Enesim_Scanline_Callback)(void *sl, int type, void *data);

typedef struct _Enesim_Scanline Enesim_Scanline; /**< Scanline Handler */
EAPI Enesim_Scanline 	*enesim_scanline_alias_new(void);
EAPI void 		enesim_scanline_delete(Enesim_Scanline *sl);

/** @} */ //End of Enesim_Scanline_Group


/** @} */ //End of Enesim_Raster_Group

/**
 * @defgroup Enesim_Vector_Group Vector
 * @{
 * @defgroup Enesim_Component_Group Component
 * @{
 */
typedef struct _Enesim_Component Enesim_Component; /**< Component Handler */
typedef struct _Enesim_Component_Reader Enesim_Component_Reader; /**< Reader Handler */
EAPI void enesim_component_delete(Enesim_Component *c);
EAPI int enesim_component_source_set(Enesim_Component *c, Enesim_Component *src);
enum
{
	ENESIM_CMD_MOVE_TO,
	ENESIM_CMD_LINE_TO,
	ENESIM_CMD_CURVE3,
	ENESIM_CMD_CURVE4,
	ENESIM_CMD_END,
	ENESIM_CMDS
};

/**
 * @defgroup Enesim_Component_Transform_Group Transform Component
 * @brief This Component applies a transformation matrix.
 * Reads vertex values from another component that is linked with the XXX
 * function and applies a transformation matrix to them.
 * @{
 */
EAPI Enesim_Component * enesim_transform_new(void);
EAPI void enesim_transform_matrix_set(Enesim_Component *c, float *matrix);
/** @} //End of Enesim_Component_Transform_Group */
/**
 * @defgroup Enesim_Component_Csv_Source_Group CSV Source Component
 * @brief This Component reads data from a csv (Comma Separated Values) file.
 * @{
 */
EAPI Enesim_Component * enesim_source_csv_new(void);
EAPI int enesim_source_csv_file_set(Enesim_Component *c, const char *path);
/** @} //End of Enesim_Component_Csv_Source_Group */

/**
 * @defgroup Enesim_Component_Path_Group Path Source Component
 * @{
 */
EAPI Enesim_Component * enesim_path_new(int num_vertices);
EAPI void enesim_path_move_to(Enesim_Component *p, int x, int y);
EAPI void enesim_path_line_to(Enesim_Component *p, int x, int y);
EAPI void enesim_path_curve3(Enesim_Component *p, float x1, float y1, float x2,
	float y2, float x3, float y3);
EAPI void enesim_path_curve4(Enesim_Component *p, float x1, float y1, float x2,
	float y2, float x3, float y3, float x4, float y4);
EAPI void enesim_path_close(Enesim_Component *p);
/** @} //End of Enesim_Component_Path_Group */
/** @} //End of Enesim_Component_Group */
/** 
 * @defgroup Enesim_Component_Reader_Group Reader
 * @{
 */
EAPI Enesim_Component_Reader * enesim_reader_new(Enesim_Component *c);
EAPI void enesim_reader_rewind(Enesim_Component_Reader *r);
EAPI int enesim_reader_vertex_get(Enesim_Component_Reader *r, float *x, float *y);
/** @} */ //End of Enesim_Component_Reader_Group
/** @} */ //End of Enesim_Vector_Group
/** @} */ //End of Enesim_Group


#endif
