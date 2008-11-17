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
 * 2. Double Buffered Canvas:
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
 * @defgroup Ekeko_Core_Group Core
 */
typedef unsigned int Ekeko_Coord;
typedef unsigned int Ekeko_Length;
typedef struct _Ekeko_Renderable Ekeko_Renderable;
typedef struct _Ekeko_Document Ekeko_Document;
typedef struct _Ekeko_Document_Type Ekeko_Document_Type;
typedef struct _Ekeko_Element Ekeko_Element;
typedef struct _Ekeko_Event Ekeko_Event;
typedef struct _Ekeko_Input Ekeko_Input;
typedef struct _Ekeko_Node Ekeko_Node;
typedef Eina_Hash Ekeko_Node_Named_Map;
typedef Eina_List Ekeko_Node_List;

typedef enum
{
	EKEKO_ERROR_NONE,
} Ekeko_Error;

extern Ekeko_Error ekeko_err; /**< */

EAPI char * ekeko_error_to_str(unsigned int err);

/**
 * @}
 * @defgroup Ekeko_Canvas_Group Canvas
 * @{ 
 */

enum
{
	EKEKO_TILER_SPLITTER,
	EKEKO_TILER_TILEBUF,
	EKEKO_TILERS
};

/**
 * @}
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
typedef enum Ekeko_Exception
{
	EKEKO_EXCEPTION_INDEX_SIZE_ERR                 = 1,
	EKEKO_EXCEPTION_DOMSTRING_SIZE_ERR             = 2,
	EKEKO_EXCEPTION_HIERARCHY_REQUEST_ERR          = 3,
	EKEKO_EXCEPTION_WRONG_DOCUMENT_ERR             = 4,
	EKEKO_EXCEPTION_INVALID_CHARACTER_ERR          = 5,
	EKEKO_EXCEPTION_NO_DATA_ALLOWED_ERR            = 6,
	EKEKO_EXCEPTION_NO_MODIFICATION_ALLOWED_ERR    = 7,
	EKEKO_EXCEPTION_NOT_FOUND_ERR                  = 8,
	EKEKO_EXCEPTION_NOT_SUPPORTED_ERR              = 9,
	EKEKO_EXCEPTION_INUSE_ATTRIBUTE_ERR            = 10,
	/* DOM Level 2 */
	EKEKO_EXCEPTION_INVALID_STATE_ERR              = 11,
	EKEKO_EXCEPTION_SYNTAX_ERR                     = 12,
	EKEKO_EXCEPTION_INVALID_MODIFICATION_ERR       = 13,
	EKEKO_EXCEPTION_NAMESPACE_ERR                  = 14,
	EKEKO_EXCEPTION_INVALID_ACCESS_ERR             = 15, 
} Ekeko_Exception;

typedef struct _Ekeko_Class Ekeko_Class;

#include "ekeko_main.h"
#include "ekeko_value.h"
#include "ekeko_node.h"
#include "ekeko_document.h"
#include "ekeko_attribute.h"
#include "ekeko_input.h"
#include "ekeko_event.h"
#include "ekeko_element.h"
#include "ekeko_class.h"
#include "ekeko_canvas.h"
#include "ekeko_renderable.h"

#endif /*EKEKO_H_*/
