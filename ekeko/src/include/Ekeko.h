#ifndef EKEKO_H_
#define EKEKO_H_

#include <Eina.h>
#include <Enesim.h>
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
typedef struct _Ekeko_Object Ekeko_Object;
typedef Eina_Bool (*Ekeko_Object_Cmp_Func)(Ekeko_Object *o, void *data);

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
typedef enum Edom_Exception
{
	EDOM_EXCEPTION_INDEX_SIZE_ERR                 = 1,
	EDOM_EXCEPTION_DOMSTRING_SIZE_ERR             = 2,
	EDOM_EXCEPTION_HIERARCHY_REQUEST_ERR          = 3,
	EDOM_EXCEPTION_WRONG_DOCUMENT_ERR             = 4,
	EDOM_EXCEPTION_INVALID_CHARACTER_ERR          = 5,
	EDOM_EXCEPTION_NO_DATA_ALLOWED_ERR            = 6,
	EDOM_EXCEPTION_NO_MODIFICATION_ALLOWED_ERR    = 7,
	EDOM_EXCEPTION_NOT_FOUND_ERR                  = 8,
	EDOM_EXCEPTION_NOT_SUPPORTED_ERR              = 9,
	EDOM_EXCEPTION_INUSE_ATTRIBUTE_ERR            = 10,
	/* DOM Level 2 */
	EDOM_EXCEPTION_INVALID_STATE_ERR              = 11,
	EDOM_EXCEPTION_SYNTAX_ERR                     = 12,
	EDOM_EXCEPTION_INVALID_MODIFICATION_ERR       = 13,
	EDOM_EXCEPTION_NAMESPACE_ERR                  = 14,
	EDOM_EXCEPTION_INVALID_ACCESS_ERR             = 15, 
} Edom_Exception;

typedef enum _Edom_Attribute_Flags
{
	EDOM_ATTRIBUTE_ABSOLUTE  = (1 << 0),
	EDOM_ATTRIBUTE_RELATIVE  = (1 << 1),
	EDOM_ATTRIBUTE_INHERITED = (1 << 2),
} Edom_Attribute_Flag;

typedef enum _Edom_Attribute_Type
{
	EDOM_ATTRIBUTE_NONE, /**< Used when the property is uninitialized */
	EDOM_ATTRIBUTE_INT, /**< The value of the property is an integer */
	EDOM_ATTRIBUTE_BOOL, /**< The value of the property is a boolean (Eina_Bool) */
	EDOM_ATTRIBUTE_CHAR, /**< The value of the property is a char */
	EDOM_ATTRIBUTE_FLOAT, /**< The value of the property is a float */
	EDOM_ATTRIBUTE_DOUBLE, /**< The value of the property is a double */
	EDOM_ATTRIBUTE_SHORT, /**< The value of the property is a short */
	EDOM_ATTRIBUTE_LONG, /**< The value of the property is a long */
	EDOM_ATTRIBUTE_POINTER, /**< The value of the property is a pointer (void *) */
	EDOM_ATTRIBUTE_STRING, /**< The value of the property is a string (char *) */	
} Edom_Attribute_Type;


typedef struct _Edom_Class Edom_Class;
typedef struct _Edom_Element_Private Edom_Element_Private;
typedef struct _Edom_Element
{
	Edom_Element_Private *priv;
} Edom_Element;

typedef enum _Edom_Attribute_Type
{
	EDOM_ATTRIBUTE_INT,
	EDOM_ATTRIBUTE_RECTANGLE,
	// EDOM_ATTRIBUTE_ELEMENT, How to handle the compare? per pointer + changed flag?
	EDOM_ATTRIBUTES,
} Edom_Attribute_Type;

typedef struct _Edom_Element Edom_Element;
typedef struct _Edom_Attribute Edom_Attribute;
typedef struct _Edom_Value
{
	Edom_Attribute_Type type;
	union 	{
		int i;
		Eina_Rectangle r;
		unsigned char *s;
		unsigned char c;
		Edom_Element *e;
	} v;
} Edom_Value;

typedef void (*Edom_Attribute_Update)(Edom_Element *, const Edom_Value *, const Edom_Value *, void *);
typedef Eina_Bool (*Edom_Value_Compare)(const Edom_Value *a, const Edom_Value *b);

void edom_init(void);
void edom_shutdown(void);

Edom_Element * edom_element_new(void);
void edom_element_process(Edom_Element *e);
void edom_element_attribute_remove(Edom_Element *e, const char *name);
void edom_element_attribute_add(Edom_Element *e, const char *name, Edom_Attribute_Type type,
		Edom_Value *def, Edom_Attribute_Update cb, void *data);
Eina_Bool edom_element_attribute_set(Edom_Element *e, const char *name, Edom_Value *v);
Eina_Bool edom_element_attribute_get(Edom_Element *e, const char *name, Edom_Value *v);

void edom_value_int_from(Edom_Value *v, int i);
void edom_value_rectangle_from(Edom_Value *v, Eina_Rectangle *r);
void edom_value_string_from(Edom_Value *v, const char *string);



#include "ekeko_value.h"
#include "ekeko_class.h"
#include "ekeko_element.h"
#include "ekeko_canvas.h"
#include "ekeko_renderable.h"
#include "ekeko_input.h"

#endif /*EKEKO_H_*/
