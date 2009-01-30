#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
/**
 * @mainpage Ekeko
 * @section intro Introduction
 * The idea behind this library is to code abstract objects, so any other canvas
 * libraries like evas itself can be built on top of this, the engine + objects (both)
 * must be coded on the upper library, this is just usefullnes without it.
 * Use cases:
 * 1. Filter Objects:
 * The filter object should receive the rectangles where to draw
 * get the pixels from the canvas surface and then filter them on its own surface for later bliting
 * or directly on the target surface.
 *
 * 2. Double Buffered Ekeko_Canvas:
 * The canvas should have a surface attached i.e the back buffer and another surface on client side
 * to receive areas changed after the process of all objects on the canvas, i.e keep the areas that
 * changed buffered and then blit them from the backbuffer to the target surface.
 *
 * 3. Mirror objects, i.e objects that resembles other objects
 * There might be some kind of callback definitions whenever an object changes, something like object
 * listeners.
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


/**
 * Explanaition:
 * Use Cases:
 * 1. Relative Values
 * parent has value X1 and child has f(X1) (i.e relative) for attribute A1
 * you do attribute_set(parent, A1, X2)
 * attribute_get(child, A1) should return f(X2) not X1
 * that implies that when an attribute is set to be relative, and the parent changes
 * we should check that any of the childs has an attribute marked as relative and
 * call the attribute set on the child based on that
 *
 * 2. Absolute Values (easy)
 * parent has value X1 and child has X2 for attribute A1
 * attribute_set(parent, A1, X3)
 * attribute_get(child, A1) returns X2
 * we need to always store the attribute that was on the previous iteration
 *
 *
 * TODO:
 * Add support to load css files from a file, same thing for animations, etc
 * Merge this into Ekeko? so all the other DOM interfaces can be done? events, etc
 * if so, add another base object, like renderable, so it has a bounding box, etc
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
