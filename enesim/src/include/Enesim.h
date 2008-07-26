#ifndef ENESIM_H_
#define ENESIM_H_

/**
 * @mainpage Enesim
 * @section intro Introduction
 * @image html raster.png 
 * Enesim is a library that ...
 * 
 * @file
 * @brief Enesim API
 * @defgroup Enesim_Group API
 * @{
 * 
 * In Enesim every intermediate surface format is argb8888_pre, so all surface
 * modes should know how to convert to/from argb8888_pre
 * 
 * To add a new surface format you should follow the next steps:
 * 1. Add a new entry to Enesim_Surface_Format enumaration in enesim_surface.h
 * 2. Create a new ENESIM_SURFACE_FORMAT(name, [yes | no]) entry into
 *    configure.in 
 * 3. Create a data struct for that format, every plane should be named as 
 *    planeX in enesim_surface.h
 * 4. Go to enesim_generator.c and follow the instructions on the beginning of
 *    the file
 * 5. Add the drawer to the array of drawers in enesim_drawer.c, with the
 *    conditional building of course
 * 6. 
 * 
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "enesim_main.h"
#include "enesim_surface.h"
#include "enesim_drawer.h"
#include "enesim_renderer.h"
#include "enesim_converter.h"
#include "enesim_rasterizer.h"
#include "enesim_transformer.h"

#ifdef __cplusplus
}
#endif
	
/** @} */
#endif
