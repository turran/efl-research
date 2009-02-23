#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
/**
 * @mainpage Ekeko
 * @section intro Introduction
 * The idea behind this library is to code abstract objects, so any other canvas
 * libraries like evas itself can be built on top of this, the engine + objects (both)
 * must be coded on the upper library, this is just usefulness without it.
 *
 * @todo
 * + Add support for group of objects
 * + Add support for clip rectangles
 * + Maybe make the subcanvas part of ekeko itslef?
 * + As we have object events, why dont canvas events?
 * + Add a way to notify if an object has changed
 * + Add two lists to an Ekeko_Canvas to keep track of the valid and
 * invalid objects. Like objects outside the canvas, objects that are invisible
 * and dont handle events, etc.
 *
 *
 * @file
 * @brief Ekeko API
 * @defgroup Ekeko_Group API
 * @{
 *
 */

typedef struct _Ekeko_Type Ekeko_Type;
typedef struct _Ekeko_Object Ekeko_Object;
typedef struct _Ekeko_Canvas Ekeko_Canvas;
typedef struct _Ekeko_Renderable Ekeko_Renderable;
typedef struct _Ekeko_Input Ekeko_Input;

#include "ekeko_main.h"
#include "ekeko_value.h"
#include "ekeko_property.h"
#include "ekeko_type.h"
#include "ekeko_event.h"
#include "ekeko_object.h"
#include "ekeko_input.h"
#include "ekeko_renderable.h"
#include "ekeko_canvas.h"

/**
 * @}
 */

#endif /*EKEKO_H_*/
